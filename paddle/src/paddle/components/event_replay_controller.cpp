#include <paddle/components/event_replay_controller.hpp>

#include <paddle/components/client.hpp>

#include <paddle/handlers/address_handler_base.hpp>
#include <paddle/handlers/api_key_handler_base.hpp>
#include <paddle/handlers/business_handler_base.hpp>
#include <paddle/handlers/client_token_handler_base.hpp>
#include <paddle/handlers/customer_handler_base.hpp>
#include <paddle/handlers/payment_method_handler_base.hpp>
#include <paddle/handlers/price_handler_base.hpp>
#include <paddle/handlers/product_handler_base.hpp>
#include <paddle/handlers/subscription_handler_base.hpp>
#include <paddle/handlers/transaction_handler_base.hpp>

#include <paddle/handlers/handlers.hpp>

#include <paddle/types/api_keys.hpp>
#include <paddle/types/client_token.hpp>
#include <paddle/types/customers.hpp>
#include <paddle/types/events.hpp>
#include <paddle/types/price.hpp>
#include <paddle/types/product.hpp>
#include <paddle/types/subscriptions.hpp>
#include <paddle/types/transactions.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/concurrent/background_task_storage.hpp>
#include <userver/formats/serialize/to.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/exceptions.hpp>
#include <userver/tracing/span.hpp>
#include <userver/utils/cpu_relax.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include <fmt/format.h>
#include <iostream>

namespace paddle::components {

namespace tracing = userver::tracing;

namespace {

constexpr auto kEventPerBatch = 200;
constexpr auto kCpuRelaxIterations = 10;
constexpr auto kReplayScopeName = "replay-events";

}  // namespace

struct EventReplayController::Impl {
    Client& client;
    handlers::Handlers handlers;

    Impl(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
        : client{context.FindComponent<Client>(config["client_name"].As<std::string>("paddle-client"))}
        , handlers{config, context} {
    }

    void Replay(const JSON& event_json, events::Event<JSON>&& event) const {
        LOG_INFO() << "Replay event: " << event.event_type << " " << event.event_id << " " << event.occurred_at;
        auto category = events::GetEventCategory(event.event_type);
        switch (category) {
            case events::EventCategory::kTransaction:
                HandleEvent(event_json, std::move(event), handlers.transaction_handler);
                break;
            case events::EventCategory::kSubscription:
                HandleEvent(event_json, std::move(event), handlers.subscription_handler);
                break;
            case events::EventCategory::kCustomer:
                HandleEvent(event_json, std::move(event), handlers.customer_handler);
                break;
            case events::EventCategory::kPaymentMethod:
                HandleEvent(event_json, std::move(event), handlers.payment_method_handler);
                break;
            case events::EventCategory::kPrice:
                HandleEvent(event_json, std::move(event), handlers.price_handler);
                break;
            case events::EventCategory::kProduct:
                HandleEvent(event_json, std::move(event), handlers.product_handler);
                break;
            case events::EventCategory::kAddress:
                HandleEvent(event_json, std::move(event), handlers.address_handler);
                break;
            case events::EventCategory::kBusiness:
                HandleEvent(event_json, std::move(event), handlers.business_handler);
                break;
            case events::EventCategory::kApiKey:
                HandleEvent(event_json, std::move(event), handlers.api_key_handler);
                break;
            case events::EventCategory::kClientToken:
                HandleEvent(event_json, std::move(event), handlers.client_token_handler);
                break;
            default:
                LOG_INFO() << "Event category not supported: " << category;
                break;
        }
    }

    template <typename T>
    void HandleEvent(const JSON& event_json, events::Event<JSON>&& event, T* handler) const {
        using EventType = typename T::EventType;
        using PayloadType = typename EventType::PayloadType;
        if (handler) {
            auto event_with_payload = events::ParsePayload<PayloadType>(std::move(event));
            handler->HandleEvent(event_json, std::move(event_with_payload));
            return;
        }
        LOG_INFO() << "No event handler configured for event category: " << T::kEventCategory;
    }

    void ReplaySince(std::string_view cursor, ReplayInfoCallback callback) const {
        LOG_INFO() << "Replay since: " << cursor;
        auto have_more = true;
        tracing::ScopeTime scope_time{kReplayScopeName};
        userver::utils::CpuRelax cpu_relax{kCpuRelaxIterations, &scope_time};
        while (have_more) {
            auto [events, next_cursor, next_have_more] = client.GetEvents(cursor, kEventPerBatch);
            for (auto& event : events) {
                // Not too efficient, but we don't want some ugly signatures
                // Anyway, this is not a hot path
                auto json = Serialize(event, userver::formats::serialize::To<JSON>());
                if (callback) {
                    callback(event);
                }
                Replay(json, std::move(event));
                cpu_relax.Relax();
            }
            cursor = next_cursor;
            have_more = next_have_more;
        }
    }
};

EventReplayController::EventReplayController(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
)
    : BaseType{config, context}
    , impl_{config, context} {
}

EventReplayController::~EventReplayController() = default;

auto EventReplayController::GetStaticConfigSchema() -> userver::yaml_config::Schema {
    return userver::yaml_config::MergeSchemas<BaseType>(fmt::format(
        R"(
type: object
description: Paddle event replay controller component
additionalProperties: false
properties:
    client_name:
        type: string
        description: |
            name of the Paddle client component (default: paddle-client)
{})",
        handlers::Handlers::GetHanderNames()
    ));
}

void EventReplayController::Replay(events::Event<JSON>&& event) const {
    auto event_json = Serialize(event, userver::formats::serialize::To<JSON>());
    impl_->Replay(event_json, std::move(event));
}

void EventReplayController::ReplaySince(std::string_view cursor, ReplayInfoCallback callback) const {
    impl_->ReplaySince(cursor, callback);
}

}  // namespace paddle::components
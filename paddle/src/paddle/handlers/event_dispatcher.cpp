#include <paddle/handlers/event_dispatcher.hpp>

#include <paddle/handlers/handlers.hpp>

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

#include <paddle/types.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/concurrent/background_task_storage.hpp>
#include <userver/http/common_headers.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/exceptions.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include <fmt/format.h>

namespace paddle::handlers {

struct EventDispatcher::Impl {
    Handlers handlers;

    bool run_in_background;
    mutable userver::concurrent::BackgroundTaskStorage bts;

    Impl(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
        : handlers{config, context}
        , run_in_background{config["run-in-background"].As<bool>(false)} {
    }

    void DispatchEvent(events::EventTypeName event_type, const EventId& event_id, JSONPtr request_json_ptr) const {
        LOG_INFO() << "Dispatching event " << event_type << " for event id " << event_id;
        auto category = events::GetEventCategory(event_type);
        switch (category) {
            case events::EventCategory::kTransaction:
                HandleEvent(request_json_ptr, handlers.transaction_handler);
                break;
            case events::EventCategory::kSubscription:
                HandleEvent(request_json_ptr, handlers.subscription_handler);
                break;
            case events::EventCategory::kCustomer:
                HandleEvent(request_json_ptr, handlers.customer_handler);
                break;
            case events::EventCategory::kPaymentMethod:
                HandleEvent(request_json_ptr, handlers.payment_method_handler);
                break;
            case events::EventCategory::kPrice:
                HandleEvent(request_json_ptr, handlers.price_handler);
                break;
            case events::EventCategory::kProduct:
                HandleEvent(request_json_ptr, handlers.product_handler);
                break;
            case events::EventCategory::kAddress:
                HandleEvent(request_json_ptr, handlers.address_handler);
                break;
            case events::EventCategory::kBusiness:
                HandleEvent(request_json_ptr, handlers.business_handler);
                break;
            case events::EventCategory::kApiKey:
                HandleEvent(request_json_ptr, handlers.api_key_handler);
                break;
            case events::EventCategory::kClientToken:
                HandleEvent(request_json_ptr, handlers.client_token_handler);
                break;
            default:
                LOG_WARNING() << "No event handler configured for event category: " << category;
                break;
        }
    }

    template <typename T>
    void HandleEvent(JSONPtr request_json_ptr, T* handler) const {
        using EventType = typename T::EventType;
        if (handler) {
            auto event = request_json_ptr->As<EventType>();
            if (run_in_background) {
                bts.AsyncDetach(
                    "handle-event",
                    [handler](JSONPtr request_json_ptr, EventType event) {
                        handler->HandleEvent(*request_json_ptr, std::move(event));
                    },
                    request_json_ptr,
                    std::move(event)
                );
            } else {
                handler->HandleEvent(*request_json_ptr, std::move(event));
            }
            return;
        }
        LOG_WARNING() << "No event handler configured for event category: " << T::kEventCategory;
    }
};

EventDispatcher::EventDispatcher(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
)
    : BaseType(config, context)
    , impl_{config, context} {
}

EventDispatcher::~EventDispatcher() = default;

auto EventDispatcher::DoHandleEvent(events::EventTypeName event_type, const EventId& event_id, JSONPtr request_json_ptr)
    const -> void {
    impl_->DispatchEvent(event_type, event_id, request_json_ptr);
}

auto EventDispatcher::GetStaticConfigSchema() -> userver::yaml_config::Schema {
    return userver::yaml_config::MergeSchemas<BaseType>(fmt::format(
        R"(
type: object
description: Paddle event dispatcher component
additionalProperties: false
properties:
    run-in-background:
        type: boolean
        description: Run event handling in background
        defaultDescription: false
{})",
        Handlers::GetHanderNames()
    ));
}

}  // namespace paddle::handlers
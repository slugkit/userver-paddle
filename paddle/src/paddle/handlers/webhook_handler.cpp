#include <paddle/handlers/webhook_handler.hpp>

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

#include <paddle/components/webhook_secret_cache.hpp>
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
#include <userver/http/common_headers.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/exceptions.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

namespace paddle::handlers {

namespace uhandlers = userver::server::handlers;

struct WebhookHandler::Impl {
    components::WebhookSecretCache& secrets_cache;
    bool run_in_background;
    mutable userver::concurrent::BackgroundTaskStorage bts;

    Handlers handlers;

    Impl(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
        : secrets_cache{context.FindComponent<components::WebhookSecretCache>(config["secrets_cache"].As<std::string>())}
        , run_in_background{config["run_in_background"].As<bool>(false)}
        , handlers{config, context} {
    }

    JSON HandleEventRequest(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value& request_json,
        [[maybe_unused]] userver::server::request::RequestContext& context
    ) const {
        if (!secrets_cache.ValidateSignature(request)) {
            throw uhandlers::Unauthorized(
                uhandlers::InternalMessage{"Invalid signature"}, uhandlers::ExternalBody{"Invalid signature"}
            );
        }
        if (!request_json.HasMember("event_type")) {
            throw uhandlers::ClientError(
                uhandlers::InternalMessage{"Invalid request: event_type is required"},
                uhandlers::ExternalBody{"Invalid request: event_type is required"}
            );
        }
        events::EventTypeName event_type;
        auto event_type_str = request_json["event_type"].As<std::string>();
        try {
            event_type = request_json["event_type"].As<events::EventTypeName>();
        } catch (const userver::formats::json::Exception& e) {
            throw uhandlers::InternalServerError(
                uhandlers::InternalMessage{fmt::format("Invalid request: event_type {} is not supported", event_type_str)
                },
                uhandlers::ExternalBody{fmt::format("Invalid request: event_type {} is not supported", event_type_str)}
            );
        }
        try {
            LOG_INFO() << "Received event: " << event_type_str;
            auto category = events::GetEventCategory(event_type);
            JSON::Builder builder;
            switch (category) {
                case events::EventCategory::kTransaction:
                    HandleEvent(request_json, handlers.transaction_handler);
                    builder["status"] = "ok";
                    break;
                case events::EventCategory::kSubscription:
                    HandleEvent(request_json, handlers.subscription_handler);
                    builder["status"] = "ok";
                    break;
                case events::EventCategory::kCustomer:
                    HandleEvent(request_json, handlers.customer_handler);
                    builder["status"] = "ok";
                    break;
                case events::EventCategory::kPaymentMethod:
                    HandleEvent(request_json, handlers.payment_method_handler);
                    builder["status"] = "ok";
                    break;
                case events::EventCategory::kPrice:
                    HandleEvent(request_json, handlers.price_handler);
                    builder["status"] = "ok";
                    break;
                case events::EventCategory::kProduct:
                    HandleEvent(request_json, handlers.product_handler);
                    builder["status"] = "ok";
                    break;
                case events::EventCategory::kAddress:
                    HandleEvent(request_json, handlers.address_handler);
                    builder["status"] = "ok";
                    break;
                case events::EventCategory::kBusiness:
                    HandleEvent(request_json, handlers.business_handler);
                    builder["status"] = "ok";
                    break;
                case events::EventCategory::kApiKey:
                    HandleEvent(request_json, handlers.api_key_handler);
                    builder["status"] = "ok";
                    break;
                case events::EventCategory::kClientToken:
                    HandleEvent(request_json, handlers.client_token_handler);
                    builder["status"] = "ok";
                    break;
                default:
                    LOG_INFO() << "Event handling not implemented for event category: " << category;
                    builder["status"] = "dubious";
                    builder["message"] =
                        fmt::format("Event handling not implemented for event category: {}", EnumToString(category));
            }
            return builder.ExtractValue();
        } catch (const std::exception& e) {
            LOG_ERROR() << "Error handling event: " << e.what();
            throw uhandlers::InternalServerError(
                uhandlers::InternalMessage{fmt::format("Error handling event: {}", e.what())},
                uhandlers::ExternalBody{fmt::format("Error handling event: {}", e.what())}
            );
        }
    }

    template <typename T>
    void HandleEvent(const userver::formats::json::Value& request_json, T* handler) const {
        if (handler) {
            auto event = request_json.As<typename T::EventType>();
            if (run_in_background) {
                bts.AsyncDetach(
                    "handle-event",
                    [handler](JSON request_json, typename T::EventType event) {
                        handler->HandleEvent(std::move(request_json), std::move(event));
                    },
                    request_json,
                    event
                );
            } else {
                handler->HandleEvent(request_json, std::move(event));
            }
            return;
        }
        LOG_INFO() << "No event handler configured for event category: " << T::kEventCategory;
    }
};

WebhookHandler::WebhookHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
)
    : BaseType{config, context}
    , impl_{config, context} {
}

WebhookHandler::~WebhookHandler() = default;

auto WebhookHandler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    userver::server::request::RequestContext& context
) const -> userver::formats::json::Value {
    return impl_->HandleEventRequest(request, request_json, context);
}

auto WebhookHandler::GetStaticConfigSchema() -> userver::yaml_config::Schema {
    return userver::yaml_config::MergeSchemas<BaseType>(fmt::format(
        R"(
type: object
description: Paddle webhook handler component
additionalProperties: false
properties:
    secrets_cache:
        type: string
        description: Component name for webhook secret cache
    run_in_background:
        type: boolean
        description: Run event handling in background
{})",
        Handlers::GetHanderNames()
    ));
}

}  // namespace paddle::handlers

#include <paddle/handlers/webhook_handler.hpp>

#include <paddle/handlers/event_sink_base.hpp>

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

    std::vector<std::string> event_sink_names;
    std::vector<EventSinkBase*> event_sinks;

    Impl(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
        : secrets_cache{context.FindComponent<components::WebhookSecretCache>(
              config["secrets-cache"].As<std::string>("paddle-webhook-secret-cache")
          )}
        , run_in_background{config["run-in-background"].As<bool>(false)}
        , event_sink_names{config["event-sinks"].As<std::vector<std::string>>({})} {
        for (const auto& event_sink_name : event_sink_names) {
            event_sinks.push_back(&context.FindComponent<EventSinkBase>(event_sink_name));
        }
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
                uhandlers::InternalMessage{
                    fmt::format("Invalid request: event_type {} is not supported", event_type_str)
                },
                uhandlers::ExternalBody{fmt::format("Invalid request: event_type {} is not supported", event_type_str)}
            );
        }
        // copy request to a shared pointer
        auto request_json_ptr = std::make_shared<JSON>(request_json);
        auto event_id = request_json["event_id"].As<EventId>();
        if (run_in_background) {
            bts.AsyncDetach(
                "sink-event",
                [this](events::EventTypeName event_type, const EventId& event_id, JSONPtr request_json_ptr) {
                    SinkEvent(event_type, event_id, request_json_ptr);
                },
                event_type,
                event_id,
                request_json_ptr
            );
        } else {
            SinkEvent(event_type, event_id, request_json_ptr);
        }
        JSON::Builder builder;
        builder["event_type"] = event_type;
        builder["event_id"] = event_id;
        builder["status"] = "ok";
        return builder.ExtractValue();
    }

    void SinkEvent(events::EventTypeName event_type, const EventId& event_id, JSONPtr request_json_ptr) const {
        for (std::size_t i = 0; i < event_sinks.size(); ++i) {
            auto event_sink = event_sinks[i];
            try {
                event_sink->HandleEvent(event_type, event_id, request_json_ptr);
            } catch (const std::exception& e) {
                LOG_ERROR() << "Error handling event with sink " << event_sink_names[i] << ": " << e.what();
                throw uhandlers::InternalServerError(
                    uhandlers::InternalMessage{
                        fmt::format("Error handling event with sink {}: {}", event_sink_names[i], e.what())
                    },
                    uhandlers::ExternalBody{
                        fmt::format("Error handling event with sink {}: {}", event_sink_names[i], e.what())
                    }
                );
            }
        }
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
    return userver::yaml_config::MergeSchemas<BaseType>(R"(
type: object
description: Paddle webhook handler component
additionalProperties: false
properties:
    secrets-cache:
        type: string
        description: Component name for webhook secret cache
        defaultDescription: paddle-webhook-secret-cache
    run-in-background:
        type: boolean
        description: Run event handling in background
        defaultDescription: false
    event-sinks:
        type: array
        description: Event sinks to handle events
        items:
            type: string
            description: |
                Component name for raw event handling
)");
}

}  // namespace paddle::handlers

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

    std::unordered_set<events::EventTypeName> allow_ignore_events;

    Impl(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
        : handlers{config, context}
        , run_in_background{config["run-in-background"].As<bool>(false)}
        , allow_ignore_events{config["allow-ignore-events"].As<std::unordered_set<events::EventTypeName>>({})} {
    }

    void DispatchEvent(
        events::EventTypeName event_type,
        const EventId& event_id,
        JSONPtr request_json_ptr,
        EventHandlingResultCallback callback
    ) const {
        LOG_INFO() << "Dispatching event " << event_type << " for event id " << event_id;
        auto category = events::GetEventCategory(event_type);
        switch (category) {
            case events::EventCategory::kTransaction:
                DispatchEvent(request_json_ptr, event_type, handlers.transaction_handler, callback);
                break;
            case events::EventCategory::kSubscription:
                DispatchEvent(request_json_ptr, event_type, handlers.subscription_handler, callback);
                break;
            case events::EventCategory::kCustomer:
                DispatchEvent(request_json_ptr, event_type, handlers.customer_handler, callback);
                break;
            case events::EventCategory::kPaymentMethod:
                DispatchEvent(request_json_ptr, event_type, handlers.payment_method_handler, callback);
                break;
            case events::EventCategory::kPrice:
                DispatchEvent(request_json_ptr, event_type, handlers.price_handler, callback);
                break;
            case events::EventCategory::kProduct:
                DispatchEvent(request_json_ptr, event_type, handlers.product_handler, callback);
                break;
            case events::EventCategory::kAddress:
                DispatchEvent(request_json_ptr, event_type, handlers.address_handler, callback);
                break;
            case events::EventCategory::kBusiness:
                DispatchEvent(request_json_ptr, event_type, handlers.business_handler, callback);
                break;
            case events::EventCategory::kApiKey:
                DispatchEvent(request_json_ptr, event_type, handlers.api_key_handler, callback);
                break;
            case events::EventCategory::kClientToken:
                DispatchEvent(request_json_ptr, event_type, handlers.client_token_handler, callback);
                break;
            default:
                LOG_WARNING() << "No event handler configured for event category: " << category;
                if (callback) {
                    callback(event_type, event_id, HandleResultStatus::kIgnored, "Unknown event category");
                }
                break;
        }
    }

    template <typename T>
    void DispatchEvent(
        JSONPtr request_json_ptr,
        events::EventTypeName event_type,
        T* handler,
        EventHandlingResultCallback callback
    ) const {
        using EventType = typename T::EventType;
        auto event_id = (*request_json_ptr)["event_id"].As<EventId>();
        if (handler) {
            if (run_in_background) {
                bts.AsyncDetach(
                    "handle-event",
                    [this](
                        T* handler,
                        JSONPtr request_json_ptr,
                        const EventId& event_id,
                        events::EventTypeName event_type,
                        EventHandlingResultCallback callback
                    ) { this->HandleEvent(handler, request_json_ptr, event_id, event_type, callback); },
                    handler,
                    request_json_ptr,
                    event_id,
                    event_type,
                    callback
                );
            } else {
                HandleEvent(handler, request_json_ptr, event_id, event_type, callback);
            }
            return;
        }
        LOG_WARNING() << "No event handler configured for event category: " << T::kEventCategory;
        if (callback) {
            if (allow_ignore_events.find(event_type) != allow_ignore_events.end()) {
                callback(event_type, event_id, HandleResultStatus::kHandled, "Event ignored");
            } else {
                callback(event_type, event_id, HandleResultStatus::kIgnored, "No event handler configured");
            }
        }
    }

    template <typename T>
    void HandleEvent(
        T* handler,
        JSONPtr request_json_ptr,
        const EventId& event_id,
        events::EventTypeName event_type,
        EventHandlingResultCallback callback
    ) const {
        using EventType = typename T::EventType;
        try {
            auto event = request_json_ptr->As<EventType>();
            HandleResult result = handler->HandleEvent(*request_json_ptr, std::move(event));
            if (callback) {
                if (result.status == HandleResultStatus::kHandled) {
                    callback(event_type, event_id, HandleResultStatus::kHandled, result.reason);
                } else {
                    if (allow_ignore_events.find(event_type) != allow_ignore_events.end()) {
                        result.status = HandleResultStatus::kHandled;
                    }
                    callback(event_type, event_id, result.status, result.reason);
                }
            }
        } catch (const std::exception& e) {
            LOG_ERROR() << "Error handling event: " << e.what();
            if (callback) {
                callback(event_type, event_id, HandleResultStatus::kError, e.what());
                return;
            }
        }
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

auto EventDispatcher::DispatchEvent(
    events::EventTypeName event_type,
    const EventId& event_id,
    JSONPtr request_json_ptr,
    EventHandlingResultCallback callback
) const -> void {
    impl_->DispatchEvent(event_type, event_id, request_json_ptr, callback);
}

auto EventDispatcher::DoHandleEvent(events::EventTypeName event_type, const EventId& event_id, JSONPtr request_json_ptr)
    const -> void {
    impl_->DispatchEvent(event_type, event_id, request_json_ptr, nullptr);
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
    allow-ignore-events:
        type: array
        description: |
            List of event types to allow ignoring. 
            If an event type is in this list, the result of the event handling will be set to kHandled
            unless an exception is thrown.
        items:
            type: string
            description: Event type
{})",
        Handlers::GetHanderNames()
    ));
}

}  // namespace paddle::handlers
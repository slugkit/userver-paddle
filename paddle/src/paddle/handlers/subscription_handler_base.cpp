#include <paddle/handlers/subscription_handler_base.hpp>

#include <paddle/types/events.hpp>
#include <paddle/types/subscriptions.hpp>

namespace paddle::handlers {

auto SubscriptionHandlerBase::HandleEvent(const JSON& request_json, EventType&& event) const -> HandleResult {
    LOG_INFO() << "Handling event: " << event.event_type << " " << event.event_id;
    switch (event.event_type) {
        case events::EventTypeName::kSubscriptionActivated:
            return HandleActivated(std::move(event));
        case events::EventTypeName::kSubscriptionCanceled:
            return HandleCanceled(std::move(event));
        case events::EventTypeName::kSubscriptionCreated: {
            auto transaction_id = request_json["data"]["transaction_id"].As<TransactionId>();
            return HandleCreated(std::move(transaction_id), std::move(event));
        }
        case events::EventTypeName::kSubscriptionImported:
            return HandleImported(std::move(event));
        case events::EventTypeName::kSubscriptionPastDue:
            return HandlePastDue(std::move(event));
        case events::EventTypeName::kSubscriptionPaused:
            return HandlePaused(std::move(event));
        case events::EventTypeName::kSubscriptionResumed:
            return HandleResumed(std::move(event));
        case events::EventTypeName::kSubscriptionUpdated:
            return HandleUpdated(std::move(event));
        default:
            LOG_INFO() << "Event handling not implemented for event type: " << event.event_type;
    }
    return HandleResult{
        HandleResultStatus::kIgnored, "Event handling not implemented for event type: " + EnumToString(event.event_type)
    };
}

auto SubscriptionHandlerBase::HandleActivated(EventType&& event) const -> HandleResult {
    return DoHandleActivated(std::move(event));
}

auto SubscriptionHandlerBase::DoHandleActivated(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto SubscriptionHandlerBase::HandleCanceled(EventType&& event) const -> HandleResult {
    return DoHandleCanceled(std::move(event));
}

auto SubscriptionHandlerBase::DoHandleCanceled(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto SubscriptionHandlerBase::HandleCreated(TransactionId&& transaction_id, EventType&& event) const -> HandleResult {
    return DoHandleCreated(std::move(transaction_id), std::move(event));
}

auto SubscriptionHandlerBase::DoHandleCreated([[maybe_unused]] TransactionId&& transaction_id, EventType&& event) const
    -> HandleResult {
    return LogEventIgnored(event);
}

auto SubscriptionHandlerBase::HandleImported(EventType&& event) const -> HandleResult {
    return DoHandleImported(std::move(event));
}

auto SubscriptionHandlerBase::DoHandleImported(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto SubscriptionHandlerBase::HandlePastDue(EventType&& event) const -> HandleResult {
    return DoHandlePastDue(std::move(event));
}

auto SubscriptionHandlerBase::DoHandlePastDue(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto SubscriptionHandlerBase::HandlePaused(EventType&& event) const -> HandleResult {
    return DoHandlePaused(std::move(event));
}

auto SubscriptionHandlerBase::DoHandlePaused(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto SubscriptionHandlerBase::HandleResumed(EventType&& event) const -> HandleResult {
    return DoHandleResumed(std::move(event));
}

auto SubscriptionHandlerBase::DoHandleResumed(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto SubscriptionHandlerBase::HandleUpdated(EventType&& event) const -> HandleResult {
    return DoHandleUpdated(std::move(event));
}

auto SubscriptionHandlerBase::DoHandleUpdated(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

}  // namespace paddle::handlers
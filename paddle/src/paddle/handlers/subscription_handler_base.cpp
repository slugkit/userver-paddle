#include <paddle/handlers/subscription_handler_base.hpp>

#include <paddle/types/events.hpp>
#include <paddle/types/subscriptions.hpp>

namespace paddle::handlers {

auto SubscriptionHandlerBase::HandleEvent(const JSON& request_json, EventType&& event) const -> void {
    LOG_INFO() << "Handling event: " << event.event_type << " " << event.event_id;
    switch (event.event_type) {
        case events::EventTypeName::kSubscriptionActivated:
            HandleActivated(std::move(event));
            break;
        case events::EventTypeName::kSubscriptionCanceled:
            HandleCanceled(std::move(event));
            break;
        case events::EventTypeName::kSubscriptionCreated: {
            auto transaction_id = request_json["data"]["transaction_id"].As<TransactionId>();
            HandleCreated(std::move(transaction_id), std::move(event));
            break;
        }
        case events::EventTypeName::kSubscriptionImported:
            HandleImported(std::move(event));
            break;
        case events::EventTypeName::kSubscriptionPastDue:
            HandlePastDue(std::move(event));
            break;
        case events::EventTypeName::kSubscriptionPaused:
            HandlePaused(std::move(event));
            break;
        case events::EventTypeName::kSubscriptionResumed:
            HandleResumed(std::move(event));
            break;
        case events::EventTypeName::kSubscriptionUpdated:
            HandleUpdated(std::move(event));
            break;
        default:
            LOG_INFO() << "Event handling not implemented for event type: " << event.event_type;
    }
}

auto SubscriptionHandlerBase::HandleActivated(EventType&& event) const -> void {
    DoHandleActivated(std::move(event));
}

auto SubscriptionHandlerBase::DoHandleActivated(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto SubscriptionHandlerBase::HandleCanceled(EventType&& event) const -> void {
    DoHandleCanceled(std::move(event));
}

auto SubscriptionHandlerBase::DoHandleCanceled(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto SubscriptionHandlerBase::HandleCreated(TransactionId&& transaction_id, EventType&& event) const -> void {
    DoHandleCreated(std::move(transaction_id), std::move(event));
}

auto SubscriptionHandlerBase::DoHandleCreated([[maybe_unused]] TransactionId&& transaction_id, EventType&& event) const
    -> void {
    LogEventIgnored(event);
}

auto SubscriptionHandlerBase::HandleImported(EventType&& event) const -> void {
    DoHandleImported(std::move(event));
}

auto SubscriptionHandlerBase::DoHandleImported(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto SubscriptionHandlerBase::HandlePastDue(EventType&& event) const -> void {
    DoHandlePastDue(std::move(event));
}

auto SubscriptionHandlerBase::DoHandlePastDue(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto SubscriptionHandlerBase::HandlePaused(EventType&& event) const -> void {
    DoHandlePaused(std::move(event));
}

auto SubscriptionHandlerBase::DoHandlePaused(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto SubscriptionHandlerBase::HandleResumed(EventType&& event) const -> void {
    DoHandleResumed(std::move(event));
}

auto SubscriptionHandlerBase::DoHandleResumed(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto SubscriptionHandlerBase::HandleUpdated(EventType&& event) const -> void {
    DoHandleUpdated(std::move(event));
}

auto SubscriptionHandlerBase::DoHandleUpdated(EventType&& event) const -> void {
    LogEventIgnored(event);
}

}  // namespace paddle::handlers
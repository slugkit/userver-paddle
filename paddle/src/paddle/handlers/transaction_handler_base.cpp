#include <paddle/handlers/transaction_handler_base.hpp>

#include <paddle/types/events.hpp>
#include <paddle/types/transactions.hpp>

#include <userver/http/common_headers.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/exceptions.hpp>

#include <fmt/format.h>

namespace paddle::handlers {

auto TransactionHandlerBase::HandleEvent([[maybe_unused]] const JSON& request_json, EventType&& event) const
    -> HandleResult {
    LOG_INFO() << "Handling event: " << event.event_type << " " << event.event_id;
    switch (event.event_type) {
        case events::EventTypeName::kTransactionBilled:
            return HandleBilled(std::move(event));
        case events::EventTypeName::kTransactionCanceled:
            return HandleCanceled(std::move(event));
        case events::EventTypeName::kTransactionCompleted:
            return HandleCompleted(std::move(event));
        case events::EventTypeName::kTransactionCreated:
            return HandleCreated(std::move(event));
        case events::EventTypeName::kTransactionPaid:
            return HandlePaid(std::move(event));
        case events::EventTypeName::kTransactionPastDue:
            return HandlePastDue(std::move(event));
        case events::EventTypeName::kTransactionPaymentFailed:
            return HandlePaymentFailed(std::move(event));
        case events::EventTypeName::kTransactionReady:
            return HandleReady(std::move(event));
        case events::EventTypeName::kTransactionRevised:
            return HandleRevised(std::move(event));
        case events::EventTypeName::kTransactionUpdated:
            return HandleUpdated(std::move(event));
        default:
            LOG_INFO() << "Event handling not implemented for event type: " << event.event_type;
    }
    return HandleResult{
        HandleResultStatus::kIgnored, "Event handling not implemented for event type: " + EnumToString(event.event_type)
    };
}

auto TransactionHandlerBase::HandleBilled(EventType&& event) const -> HandleResult {
    return DoHandleBilled(std::move(event));
}

auto TransactionHandlerBase::DoHandleBilled(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto TransactionHandlerBase::HandleCanceled(EventType&& event) const -> HandleResult {
    return DoHandleCanceled(std::move(event));
}

auto TransactionHandlerBase::DoHandleCanceled(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto TransactionHandlerBase::HandleCompleted(EventType&& event) const -> HandleResult {
    return DoHandleCompleted(std::move(event));
}

auto TransactionHandlerBase::DoHandleCompleted(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto TransactionHandlerBase::HandleCreated(EventType&& event) const -> HandleResult {
    return DoHandleCreated(std::move(event));
}

auto TransactionHandlerBase::DoHandleCreated(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto TransactionHandlerBase::HandlePaid(EventType&& event) const -> HandleResult {
    return DoHandlePaid(std::move(event));
}

auto TransactionHandlerBase::DoHandlePaid(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto TransactionHandlerBase::HandlePastDue(EventType&& event) const -> HandleResult {
    return DoHandlePastDue(std::move(event));
}

auto TransactionHandlerBase::DoHandlePastDue(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto TransactionHandlerBase::HandlePaymentFailed(EventType&& event) const -> HandleResult {
    return DoHandlePaymentFailed(std::move(event));
}

auto TransactionHandlerBase::DoHandlePaymentFailed(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto TransactionHandlerBase::HandleReady(EventType&& event) const -> HandleResult {
    return DoHandleReady(std::move(event));
}

auto TransactionHandlerBase::DoHandleReady(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto TransactionHandlerBase::HandleRevised(EventType&& event) const -> HandleResult {
    return DoHandleRevised(std::move(event));
}

auto TransactionHandlerBase::DoHandleRevised(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto TransactionHandlerBase::HandleUpdated(EventType&& event) const -> HandleResult {
    return DoHandleUpdated(std::move(event));
}

auto TransactionHandlerBase::DoHandleUpdated(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

}  // namespace paddle::handlers

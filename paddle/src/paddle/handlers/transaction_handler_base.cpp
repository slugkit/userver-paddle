#include <paddle/handlers/transaction_handler_base.hpp>

#include <paddle/types/events.hpp>
#include <paddle/types/transactions.hpp>

#include <userver/http/common_headers.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/exceptions.hpp>

#include <fmt/format.h>

namespace paddle::handlers {

auto TransactionHandlerBase::HandleEvent([[maybe_unused]] const JSON& request_json, EventType&& event) const -> void {
    LOG_INFO() << "Handling event: " << event.event_type << " " << event.event_id;
    switch (event.event_type) {
        case events::EventTypeName::kTransactionBilled:
            HandleBilled(std::move(event));
            break;
        case events::EventTypeName::kTransactionCanceled:
            HandleCanceled(std::move(event));
            break;
        case events::EventTypeName::kTransactionCompleted:
            HandleCompleted(std::move(event));
            break;
        case events::EventTypeName::kTransactionCreated:
            HandleCreated(std::move(event));
            break;
        case events::EventTypeName::kTransactionPaid:
            HandlePaid(std::move(event));
            break;
        case events::EventTypeName::kTransactionPastDue:
            HandlePastDue(std::move(event));
            break;
        case events::EventTypeName::kTransactionPaymentFailed:
            HandlePaymentFailed(std::move(event));
            break;
        case events::EventTypeName::kTransactionReady:
            HandleReady(std::move(event));
            break;
        case events::EventTypeName::kTransactionRevised:
            HandleRevised(std::move(event));
            break;
        case events::EventTypeName::kTransactionUpdated:
            HandleUpdated(std::move(event));
            break;
        default:
            LOG_INFO() << "Event handling not implemented for event type: " << event.event_type;
    }
}

auto TransactionHandlerBase::HandleBilled(EventType&& event) const -> void {
    DoHandleBilled(std::move(event));
}

auto TransactionHandlerBase::DoHandleBilled(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto TransactionHandlerBase::HandleCanceled(EventType&& event) const -> void {
    DoHandleCanceled(std::move(event));
}

auto TransactionHandlerBase::DoHandleCanceled(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto TransactionHandlerBase::HandleCompleted(EventType&& event) const -> void {
    DoHandleCompleted(std::move(event));
}

auto TransactionHandlerBase::DoHandleCompleted(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto TransactionHandlerBase::HandleCreated(EventType&& event) const -> void {
    DoHandleCreated(std::move(event));
}

auto TransactionHandlerBase::DoHandleCreated(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto TransactionHandlerBase::HandlePaid(EventType&& event) const -> void {
    DoHandlePaid(std::move(event));
}

auto TransactionHandlerBase::DoHandlePaid(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto TransactionHandlerBase::HandlePastDue(EventType&& event) const -> void {
    DoHandlePastDue(std::move(event));
}

auto TransactionHandlerBase::DoHandlePastDue(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto TransactionHandlerBase::HandlePaymentFailed(EventType&& event) const -> void {
    DoHandlePaymentFailed(std::move(event));
}

auto TransactionHandlerBase::DoHandlePaymentFailed(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto TransactionHandlerBase::HandleReady(EventType&& event) const -> void {
    DoHandleReady(std::move(event));
}

auto TransactionHandlerBase::DoHandleReady(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto TransactionHandlerBase::HandleRevised(EventType&& event) const -> void {
    DoHandleRevised(std::move(event));
}

auto TransactionHandlerBase::DoHandleRevised(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto TransactionHandlerBase::HandleUpdated(EventType&& event) const -> void {
    DoHandleUpdated(std::move(event));
}

auto TransactionHandlerBase::DoHandleUpdated(EventType&& event) const -> void {
    LogEventIgnored(event);
}

}  // namespace paddle::handlers

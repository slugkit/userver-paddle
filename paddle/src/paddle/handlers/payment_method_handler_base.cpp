#include <paddle/handlers/payment_method_handler_base.hpp>

#include <paddle/types/events.hpp>
#include <paddle/types/payment_method.hpp>

#include <userver/http/common_headers.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/exceptions.hpp>

#include <fmt/format.h>

namespace paddle::handlers {

auto PaymentMethodHandlerBase::HandleEvent([[maybe_unused]] const JSON& request_json, EventType&& event) const
    -> HandleResult {
    LOG_INFO() << "Handling event: " << event.event_type << " " << event.event_id;
    switch (event.event_type) {
        case events::EventTypeName::kPaymentMethodSaved:
            return HandleSaved(std::move(event));
        case events::EventTypeName::kPaymentMethodDeleted:
            return HandleDeleted(std::move(event));
        default:
            LOG_INFO() << "Event handling not implemented for event type: " << event.event_type;
    }
    return HandleResult{
        HandleResultStatus::kIgnored, "Event handling not implemented for event type: " + EnumToString(event.event_type)
    };
}

auto PaymentMethodHandlerBase::HandleSaved(EventType&& event) const -> HandleResult {
    return DoHandleSaved(std::move(event));
}

auto PaymentMethodHandlerBase::DoHandleSaved(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto PaymentMethodHandlerBase::HandleDeleted(EventType&& event) const -> HandleResult {
    return DoHandleDeleted(std::move(event));
}

auto PaymentMethodHandlerBase::DoHandleDeleted(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

}  // namespace paddle::handlers
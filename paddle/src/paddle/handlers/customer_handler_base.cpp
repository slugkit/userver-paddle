#include <paddle/handlers/customer_handler_base.hpp>

#include <paddle/types/customers.hpp>
#include <paddle/types/events.hpp>

namespace paddle::handlers {

auto CustomerHandlerBase::HandleEvent([[maybe_unused]] const JSON& request_json, EventType&& event) const
    -> HandleResult {
    LOG_INFO() << "Handling event: " << event.event_type << " " << event.event_id;
    switch (event.event_type) {
        case events::EventTypeName::kCustomerCreated:
            return HandleCreated(std::move(event));
        case events::EventTypeName::kCustomerImported:
            return HandleImported(std::move(event));
        case events::EventTypeName::kCustomerUpdated:
            return HandleUpdated(std::move(event));
        default:
            LOG_INFO() << "Event handling not implemented for event type: " << event.event_type;
    }
    return HandleResult{
        HandleResultStatus::kIgnored, "Event handling not implemented for event type: " + EnumToString(event.event_type)
    };
}

auto CustomerHandlerBase::HandleCreated(EventType&& event) const -> HandleResult {
    return DoHandleCreated(std::move(event));
}

auto CustomerHandlerBase::DoHandleCreated(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto CustomerHandlerBase::HandleImported(EventType&& event) const -> HandleResult {
    return DoHandleImported(std::move(event));
}

auto CustomerHandlerBase::DoHandleImported(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto CustomerHandlerBase::HandleUpdated(EventType&& event) const -> HandleResult {
    return DoHandleUpdated(std::move(event));
}

auto CustomerHandlerBase::DoHandleUpdated(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

}  // namespace paddle::handlers
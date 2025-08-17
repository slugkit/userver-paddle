#include <paddle/handlers/customer_handler_base.hpp>

#include <paddle/types/customers.hpp>
#include <paddle/types/events.hpp>

namespace paddle::handlers {

auto CustomerHandlerBase::HandleEvent([[maybe_unused]] const JSON& request_json, EventType&& event) const -> void {
    LOG_INFO() << "Handling event: " << event.event_type << " " << event.event_id;
    switch (event.event_type) {
        case events::EventTypeName::kCustomerCreated:
            HandleCreated(std::move(event));
            break;
        case events::EventTypeName::kCustomerImported:
            HandleImported(std::move(event));
            break;
        case events::EventTypeName::kCustomerUpdated:
            HandleUpdated(std::move(event));
            break;
        default:
            LOG_INFO() << "Event handling not implemented for event type: " << event.event_type;
    }
}

void CustomerHandlerBase::HandleCreated(EventType&& event) const {
    DoHandleCreated(std::move(event));
}

void CustomerHandlerBase::DoHandleCreated(EventType&& event) const {
    LogEventIgnored(event);
}

void CustomerHandlerBase::HandleImported(EventType&& event) const {
    DoHandleImported(std::move(event));
}

void CustomerHandlerBase::DoHandleImported(EventType&& event) const {
    LogEventIgnored(event);
}

void CustomerHandlerBase::HandleUpdated(EventType&& event) const {
    DoHandleUpdated(std::move(event));
}

void CustomerHandlerBase::DoHandleUpdated(EventType&& event) const {
    LogEventIgnored(event);
}

}  // namespace paddle::handlers
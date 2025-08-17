#include <paddle/handlers/address_handler_base.hpp>

#include <paddle/types/customers.hpp>

#include <userver/http/common_headers.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/exceptions.hpp>

#include <fmt/format.h>

namespace paddle::handlers {

auto AddressHandlerBase::HandleEvent([[maybe_unused]] const JSON& request_json, EventType&& event) const -> void {
    LOG_INFO() << "Handling event: " << event.event_type << " " << event.event_id;
    switch (event.event_type) {
        case events::EventTypeName::kAddressCreated:
            HandleCreated(std::move(event));
            break;
        case events::EventTypeName::kAddressImported:
            HandleImported(std::move(event));
            break;
        case events::EventTypeName::kAddressUpdated:
            HandleUpdated(std::move(event));
            break;
        default:
            LOG_INFO() << "Event handling not implemented for event type: " << event.event_type;
    }
}

auto AddressHandlerBase::HandleCreated(EventType&& event) const -> void {
    DoHandleCreated(std::move(event));
}

auto AddressHandlerBase::DoHandleCreated(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto AddressHandlerBase::HandleImported(EventType&& event) const -> void {
    DoHandleImported(std::move(event));
}

auto AddressHandlerBase::DoHandleImported(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto AddressHandlerBase::HandleUpdated(EventType&& event) const -> void {
    DoHandleUpdated(std::move(event));
}

auto AddressHandlerBase::DoHandleUpdated(EventType&& event) const -> void {
    LogEventIgnored(event);
}

}  // namespace paddle::components
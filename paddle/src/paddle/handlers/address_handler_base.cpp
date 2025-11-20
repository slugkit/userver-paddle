#include <paddle/handlers/address_handler_base.hpp>

#include <paddle/types/customers.hpp>

#include <userver/http/common_headers.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/exceptions.hpp>

#include <fmt/format.h>

namespace paddle::handlers {

auto AddressHandlerBase::HandleEvent([[maybe_unused]] const JSON& request_json, EventType&& event) const
    -> HandleResult {
    LOG_INFO() << "Handling event: " << event.event_type << " " << event.event_id;
    switch (event.event_type) {
        case events::EventTypeName::kAddressCreated:
            return HandleCreated(std::move(event));
        case events::EventTypeName::kAddressImported:
            return HandleImported(std::move(event));
        case events::EventTypeName::kAddressUpdated:
            return HandleUpdated(std::move(event));
        default:
            LOG_INFO() << "Event handling not implemented for event type: " << event.event_type;
    }
    return HandleResult{
        HandleResultStatus::kIgnored, "Event handling not implemented for event type: " + EnumToString(event.event_type)
    };
}

auto AddressHandlerBase::HandleCreated(EventType&& event) const -> HandleResult {
    return DoHandleCreated(std::move(event));
}

auto AddressHandlerBase::DoHandleCreated(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto AddressHandlerBase::HandleImported(EventType&& event) const -> HandleResult {
    return DoHandleImported(std::move(event));
}

auto AddressHandlerBase::DoHandleImported(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto AddressHandlerBase::HandleUpdated(EventType&& event) const -> HandleResult {
    return DoHandleUpdated(std::move(event));
}

auto AddressHandlerBase::DoHandleUpdated(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

}  // namespace paddle::handlers
#include <paddle/handlers/business_handler_base.hpp>

#include <paddle/types/customers.hpp>
#include <paddle/types/events.hpp>

#include <userver/http/common_headers.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/exceptions.hpp>

#include <fmt/format.h>

namespace paddle::handlers {

auto BusinessHandlerBase::HandleEvent([[maybe_unused]] const JSON& request_json, EventType&& event) const
    -> HandleResult {
    LOG_INFO() << "Handling event: " << event.event_type << " " << event.event_id;
    switch (event.event_type) {
        case events::EventTypeName::kBusinessCreated:
            return HandleCreated(std::move(event));
        case events::EventTypeName::kBusinessImported:
            return HandleImported(std::move(event));
        case events::EventTypeName::kBusinessUpdated:
            return HandleUpdated(std::move(event));
        default:
            LOG_INFO() << "Event handling not implemented for event type: " << event.event_type;
    }
    return HandleResult{
        HandleResultStatus::kIgnored, "Event handling not implemented for event type: " + EnumToString(event.event_type)
    };
}

auto BusinessHandlerBase::HandleCreated(EventType&& event) const -> HandleResult {
    return DoHandleCreated(std::move(event));
}

auto BusinessHandlerBase::DoHandleCreated(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto BusinessHandlerBase::HandleImported(EventType&& event) const -> HandleResult {
    return DoHandleImported(std::move(event));
}

auto BusinessHandlerBase::DoHandleImported(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto BusinessHandlerBase::HandleUpdated(EventType&& event) const -> HandleResult {
    return DoHandleUpdated(std::move(event));
}

auto BusinessHandlerBase::DoHandleUpdated(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

}  // namespace paddle::handlers
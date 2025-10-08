#include <paddle/handlers/client_token_handler_base.hpp>

#include <paddle/types/client_token.hpp>

#include <userver/http/common_headers.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/exceptions.hpp>

#include <fmt/format.h>

namespace paddle::handlers {

auto ClientTokenHandlerBase::HandleEvent([[maybe_unused]] const JSON& request_json, EventType&& event) const
    -> HandleResult {
    LOG_INFO() << "Handling event: " << event.event_type << " " << event.event_id;
    switch (event.event_type) {
        case events::EventTypeName::kClientTokenCreated:
            return HandleCreated(std::move(event));
        case events::EventTypeName::kClientTokenRevoked:
            return HandleRevoked(std::move(event));
        case events::EventTypeName::kClientTokenUpdated:
            return HandleUpdated(std::move(event));
        default:
            LOG_INFO() << "Event handling not implemented for event type: " << event.event_type;
    }
    return HandleResult{
        HandleResultStatus::kIgnored, "Event handling not implemented for event type: " + EnumToString(event.event_type)
    };
}

auto ClientTokenHandlerBase::HandleCreated(EventType&& event) const -> HandleResult {
    return DoHandleCreated(std::move(event));
}

auto ClientTokenHandlerBase::DoHandleCreated(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto ClientTokenHandlerBase::HandleRevoked(EventType&& event) const -> HandleResult {
    return DoHandleRevoked(std::move(event));
}

auto ClientTokenHandlerBase::DoHandleRevoked(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto ClientTokenHandlerBase::HandleUpdated(EventType&& event) const -> HandleResult {
    return DoHandleUpdated(std::move(event));
}

auto ClientTokenHandlerBase::DoHandleUpdated(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

}  // namespace paddle::handlers
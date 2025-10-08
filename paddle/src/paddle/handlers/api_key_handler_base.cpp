#include <paddle/handlers/api_key_handler_base.hpp>

#include <paddle/types/api_keys.hpp>

#include <userver/http/common_headers.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/exceptions.hpp>

#include <fmt/format.h>

namespace paddle::handlers {

auto ApiKeyHandlerBase::HandleEvent([[maybe_unused]] const JSON& request_json, EventType&& event) const
    -> HandleResult {
    LOG_INFO() << "Handling event: " << event.event_type << " " << event.event_id;
    switch (event.event_type) {
        case events::EventTypeName::kApiKeyCreated:
            return HandleCreated(std::move(event));
        case events::EventTypeName::kApiKeyExpired:
            return HandleExpired(std::move(event));
        case events::EventTypeName::kApiKeyExpiring:
            return HandleExpiring(std::move(event));
        case events::EventTypeName::kApiKeyRevoked:
            return HandleRevoked(std::move(event));
        case events::EventTypeName::kApiKeyUpdated:
            return HandleUpdated(std::move(event));
        default:
            LOG_INFO() << "Event handling not implemented for event type: " << event.event_type;
    }
    return HandleResult{
        HandleResultStatus::kIgnored, "Event handling not implemented for event type: " + EnumToString(event.event_type)
    };
}

auto ApiKeyHandlerBase::HandleCreated(EventType&& event) const -> HandleResult {
    return DoHandleCreated(std::move(event));
}

auto ApiKeyHandlerBase::DoHandleCreated(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto ApiKeyHandlerBase::HandleExpired(EventType&& event) const -> HandleResult {
    return DoHandleExpired(std::move(event));
}

auto ApiKeyHandlerBase::DoHandleExpired(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto ApiKeyHandlerBase::HandleExpiring(EventType&& event) const -> HandleResult {
    return DoHandleExpiring(std::move(event));
}

auto ApiKeyHandlerBase::DoHandleExpiring(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto ApiKeyHandlerBase::HandleRevoked(EventType&& event) const -> HandleResult {
    return DoHandleRevoked(std::move(event));
}

auto ApiKeyHandlerBase::DoHandleRevoked(EventType&& event) const -> HandleResult {
    LogEventIgnored(event);
    return HandleResult{HandleResultStatus::kIgnored, "Event ignored"};
}

auto ApiKeyHandlerBase::HandleUpdated(EventType&& event) const -> HandleResult {
    return DoHandleUpdated(std::move(event));
}

auto ApiKeyHandlerBase::DoHandleUpdated(EventType&& event) const -> HandleResult {
    LogEventIgnored(event);
    return HandleResult{HandleResultStatus::kIgnored, "Event ignored"};
}

}  // namespace paddle::handlers

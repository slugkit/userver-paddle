#include <paddle/handlers/api_key_handler_base.hpp>

#include <paddle/types/api_keys.hpp>

#include <userver/http/common_headers.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/exceptions.hpp>

#include <fmt/format.h>

namespace paddle::handlers {

auto ApiKeyHandlerBase::HandleEvent([[maybe_unused]] const JSON& request_json, EventType&& event) const -> void {
    LOG_INFO() << "Handling event: " << event.event_type << " " << event.event_id;
    switch (event.event_type) {
        case events::EventTypeName::kApiKeyCreated:
            HandleCreated(std::move(event));
            break;
        case events::EventTypeName::kApiKeyExpired:
            HandleExpired(std::move(event));
            break;
        case events::EventTypeName::kApiKeyExpiring:
            HandleExpiring(std::move(event));
            break;
        case events::EventTypeName::kApiKeyRevoked:
            HandleRevoked(std::move(event));
            break;
        case events::EventTypeName::kApiKeyUpdated:
            HandleUpdated(std::move(event));
            break;
        default:
            LOG_INFO() << "Event handling not implemented for event type: " << event.event_type;
    }
}

auto ApiKeyHandlerBase::HandleCreated(EventType&& event) const -> void {
    DoHandleCreated(std::move(event));
}

auto ApiKeyHandlerBase::DoHandleCreated(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto ApiKeyHandlerBase::HandleExpired(EventType&& event) const -> void {
    DoHandleExpired(std::move(event));
}

auto ApiKeyHandlerBase::DoHandleExpired(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto ApiKeyHandlerBase::HandleExpiring(EventType&& event) const -> void {
    DoHandleExpiring(std::move(event));
}

auto ApiKeyHandlerBase::DoHandleExpiring(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto ApiKeyHandlerBase::HandleRevoked(EventType&& event) const -> void {
    DoHandleRevoked(std::move(event));
}

auto ApiKeyHandlerBase::DoHandleRevoked(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto ApiKeyHandlerBase::HandleUpdated(EventType&& event) const -> void {
    DoHandleUpdated(std::move(event));
}

auto ApiKeyHandlerBase::DoHandleUpdated(EventType&& event) const -> void {
    LogEventIgnored(event);
}

}  // namespace paddle::handlers

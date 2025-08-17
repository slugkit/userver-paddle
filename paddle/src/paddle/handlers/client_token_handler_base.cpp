#include <paddle/handlers/client_token_handler_base.hpp>

#include <paddle/types/client_token.hpp>

#include <userver/http/common_headers.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/exceptions.hpp>

#include <fmt/format.h>

namespace paddle::handlers {

auto ClientTokenHandlerBase::HandleEvent([[maybe_unused]] const JSON& request_json, EventType&& event) const -> void {
    LOG_INFO() << "Handling event: " << event.event_type << " " << event.event_id;
    switch (event.event_type) {
        case events::EventTypeName::kClientTokenCreated:
            HandleCreated(std::move(event));
            break;
        case events::EventTypeName::kClientTokenRevoked:
            HandleRevoked(std::move(event));
            break;
        case events::EventTypeName::kClientTokenUpdated:
            HandleUpdated(std::move(event));
            break;
        default:
            LOG_INFO() << "Event handling not implemented for event type: " << event.event_type;
    }
}

auto ClientTokenHandlerBase::HandleCreated(EventType&& event) const -> void {
    DoHandleCreated(std::move(event));
}

auto ClientTokenHandlerBase::DoHandleCreated(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto ClientTokenHandlerBase::HandleRevoked(EventType&& event) const -> void {
    DoHandleRevoked(std::move(event));
}

auto ClientTokenHandlerBase::DoHandleRevoked(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto ClientTokenHandlerBase::HandleUpdated(EventType&& event) const -> void {
    DoHandleUpdated(std::move(event));
}

auto ClientTokenHandlerBase::DoHandleUpdated(EventType&& event) const -> void {
    LogEventIgnored(event);
}

}  // namespace paddle::handlers
#include <paddle/handlers/product_handler_base.hpp>

#include <paddle/types/product.hpp>

#include <userver/http/common_headers.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/exceptions.hpp>

#include <fmt/format.h>

namespace paddle::handlers {

auto ProductHandlerBase::HandleEvent([[maybe_unused]] const JSON& request_json, EventType&& event) const -> void {
    LOG_INFO() << "Handling event: " << event.event_type << " " << event.event_id;
    switch (event.event_type) {
        case events::EventTypeName::kProductCreated:
            HandleCreated(std::move(event));
            break;
        case events::EventTypeName::kProductImported:
            HandleImported(std::move(event));
            break;
        case events::EventTypeName::kProductUpdated:
            HandleUpdated(std::move(event));
            break;
        default:
            LOG_INFO() << "Event handling not implemented for event type: " << event.event_type;
    }
}

auto ProductHandlerBase::HandleCreated(EventType&& event) const -> void {
    DoHandleCreated(std::move(event));
}

auto ProductHandlerBase::DoHandleCreated(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto ProductHandlerBase::HandleImported(EventType&& event) const -> void {
    DoHandleImported(std::move(event));
}

auto ProductHandlerBase::DoHandleImported(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto ProductHandlerBase::HandleUpdated(EventType&& event) const -> void {
    DoHandleUpdated(std::move(event));
}

auto ProductHandlerBase::DoHandleUpdated(EventType&& event) const -> void {
    LogEventIgnored(event);
}

}  // namespace paddle::handlers
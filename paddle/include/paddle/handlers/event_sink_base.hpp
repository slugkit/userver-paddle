#pragma once

#include <paddle/types/events.hpp>

#include <userver/components/component_base.hpp>

namespace paddle::handlers {

/// This is a base component for handling events
/// By default does nothing, but can be overridden to handle events,
/// e.g. to send events to a database or a message queue.
class EventSinkBase : public userver::components::ComponentBase {
public:
    using BaseType = userver::components::ComponentBase;
    using EventType = events::Event<>;

    using BaseType::BaseType;

    virtual ~EventSinkBase() = default;

    auto HandleEvent(events::EventTypeName event_type, const EventId& event_id, JSONPtr request_json_ptr) const
        -> void {
        DoHandleEvent(event_type, event_id, request_json_ptr);
    }

private:
    virtual auto DoHandleEvent(events::EventTypeName event_type, const EventId& event_id, JSONPtr request_json_ptr)
        const -> void = 0;
};

}  // namespace paddle::handlers
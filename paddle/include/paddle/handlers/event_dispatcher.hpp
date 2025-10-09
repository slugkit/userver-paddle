#pragma once

#include <paddle/handlers/event_sink_base.hpp>
#include <paddle/handlers/handle_result.hpp>
#include <paddle/types/events.hpp>

#include <userver/utils/fast_pimpl.hpp>

#include <functional>

namespace paddle::handlers {

/// @brief Dispatches events to all typed event handlers
/// This is a component that dispatches events to all typed event handlers,
/// that are configured in the component config.
/// It can be used as an event sink for the WebhookHandler or as a standalone component
/// to replay events from the database.
class EventDispatcher final : public EventSinkBase {
public:
    using EventHandlingResultCallback = std::function<void(
        events::EventTypeName event_type,
        const EventId& event_id,
        HandleResultStatus status,
        std::optional<std::string> status_reason
    )>;

public:
    static constexpr auto kName = "paddle-event-dispatcher";

    using BaseType = EventSinkBase;

    EventDispatcher(
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    );
    ~EventDispatcher() override;

    static auto GetStaticConfigSchema() -> userver::yaml_config::Schema;

    auto DispatchEvent(
        events::EventTypeName event_type,
        const EventId& event_id,
        JSONPtr request_json_ptr,
        EventHandlingResultCallback callback
    ) const -> void;

private:
    auto DoHandleEvent(events::EventTypeName event_type, const EventId& event_id, JSONPtr request_json_ptr) const
        -> void override;

private:
    constexpr static auto kImplSize = 288UL;
    constexpr static auto kImplAlign = 16UL;
    struct Impl;
    userver::utils::FastPimpl<Impl, kImplSize, kImplAlign> impl_;
};

}  // namespace paddle::handlers
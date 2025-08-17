#pragma once

#include <paddle/types/events.hpp>
#include <paddle/types/formats.hpp>
#include <paddle/types/fwd.hpp>

#include <userver/components/component_base.hpp>

namespace paddle::handlers {

class ApiKeyHandlerBase : public userver::components::ComponentBase {
public:
    using BaseType = userver::components::ComponentBase;
    using EventType = events::Event<api_keys::ApiKeyEventPayload>;
    constexpr static auto kEventCategory = events::EventCategory::kApiKey;

    using BaseType::BaseType;

    auto HandleEvent(const JSON& request_json, EventType&& event) const -> void;

    auto HandleCreated(EventType&& event) const -> void;
    auto HandleExpired(EventType&& event) const -> void;
    auto HandleExpiring(EventType&& event) const -> void;
    auto HandleRevoked(EventType&& event) const -> void;
    auto HandleUpdated(EventType&& event) const -> void;

private:
    virtual auto DoHandleCreated(EventType&&) const -> void;
    virtual auto DoHandleExpired(EventType&&) const -> void;
    virtual auto DoHandleExpiring(EventType&&) const -> void;
    virtual auto DoHandleRevoked(EventType&&) const -> void;
    virtual auto DoHandleUpdated(EventType&&) const -> void;
};

}  // namespace paddle::handlers
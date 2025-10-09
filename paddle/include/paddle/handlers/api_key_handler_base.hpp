#pragma once

#include <paddle/handlers/handle_result.hpp>
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

    auto HandleEvent(const JSON& request_json, EventType&& event) const -> HandleResult;

    auto HandleCreated(EventType&& event) const -> HandleResult;
    auto HandleExpired(EventType&& event) const -> HandleResult;
    auto HandleExpiring(EventType&& event) const -> HandleResult;
    auto HandleRevoked(EventType&& event) const -> HandleResult;
    auto HandleUpdated(EventType&& event) const -> HandleResult;

private:
    virtual auto DoHandleCreated(EventType&&) const -> HandleResult;
    virtual auto DoHandleExpired(EventType&&) const -> HandleResult;
    virtual auto DoHandleExpiring(EventType&&) const -> HandleResult;
    virtual auto DoHandleRevoked(EventType&&) const -> HandleResult;
    virtual auto DoHandleUpdated(EventType&&) const -> HandleResult;
};

}  // namespace paddle::handlers
#pragma once

#include <paddle/types/events.hpp>
#include <paddle/types/formats.hpp>
#include <paddle/types/fwd.hpp>

#include <userver/components/component_base.hpp>

namespace paddle::handlers {

class ClientTokenHandlerBase : public userver::components::ComponentBase {
public:
    using BaseType = userver::components::ComponentBase;
    using EventType = events::Event<client_tokens::ClientToken>;
    constexpr static auto kEventCategory = events::EventCategory::kClientToken;

    using BaseType::BaseType;

    auto HandleEvent(const JSON& request_json, EventType&& event) const -> HandleResult;

    auto HandleCreated(EventType&& event) const -> HandleResult;
    auto HandleRevoked(EventType&& event) const -> HandleResult;
    auto HandleUpdated(EventType&& event) const -> HandleResult;

private:
    virtual auto DoHandleCreated(EventType&&) const -> HandleResult;
    virtual auto DoHandleRevoked(EventType&&) const -> HandleResult;
    virtual auto DoHandleUpdated(EventType&&) const -> HandleResult;
};

}  // namespace paddle::handlers
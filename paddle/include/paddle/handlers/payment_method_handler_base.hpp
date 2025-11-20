#pragma once

#include <paddle/types/events.hpp>
#include <paddle/types/formats.hpp>
#include <paddle/types/fwd.hpp>

#include <userver/components/component_base.hpp>

namespace paddle::handlers {

class PaymentMethodHandlerBase : public userver::components::ComponentBase {
public:
    using BaseType = userver::components::ComponentBase;
    using EventType = events::Event<money::PaymentMethodEventPayload>;
    constexpr static auto kEventCategory = events::EventCategory::kPaymentMethod;

    using BaseType::BaseType;

    auto HandleEvent(const JSON& request_json, EventType&& event) const -> HandleResult;

    auto HandleSaved(EventType&& event) const -> HandleResult;
    auto HandleDeleted(EventType&& event) const -> HandleResult;

private:
    virtual auto DoHandleSaved(EventType&&) const -> HandleResult;
    virtual auto DoHandleDeleted(EventType&&) const -> HandleResult;
};

}  // namespace paddle::handlers

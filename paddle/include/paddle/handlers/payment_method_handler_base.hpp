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

    auto HandleEvent(const JSON& request_json, EventType&& event) const -> void;

    auto HandleSaved(EventType&& event) const -> void;
    auto HandleDeleted(EventType&& event) const -> void;

private:
    virtual auto DoHandleSaved(EventType&&) const -> void;
    virtual auto DoHandleDeleted(EventType&&) const -> void;
};

}  // namespace paddle::handlers

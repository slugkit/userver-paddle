#pragma once

#include <paddle/types/events.hpp>
#include <paddle/types/formats.hpp>
#include <paddle/types/fwd.hpp>

#include <userver/components/component_base.hpp>

namespace paddle::handlers {

class TransactionHandlerBase : public userver::components::ComponentBase {
public:
    using BaseType = userver::components::ComponentBase;
    using EventType = events::Event<transactions::Transaction>;
    constexpr static auto kEventCategory = events::EventCategory::kTransaction;

    using BaseType::BaseType;

    auto HandleEvent(const JSON& request_json, EventType&& event) const -> void;

    auto HandleBilled(EventType&& event) const -> void;
    auto HandleCanceled(EventType&& event) const -> void;
    auto HandleCompleted(EventType&& event) const -> void;
    auto HandleCreated(EventType&& event) const -> void;
    auto HandlePaid(EventType&& event) const -> void;
    auto HandlePastDue(EventType&& event) const -> void;
    auto HandlePaymentFailed(EventType&& event) const -> void;
    auto HandleReady(EventType&& event) const -> void;
    auto HandleRevised(EventType&& event) const -> void;
    auto HandleUpdated(EventType&& event) const -> void;

private:
    virtual auto DoHandleBilled(EventType&&) const -> void;
    virtual auto DoHandleCanceled(EventType&&) const -> void;
    virtual auto DoHandleCompleted(EventType&&) const -> void;
    virtual auto DoHandleCreated(EventType&&) const -> void;
    virtual auto DoHandlePaid(EventType&&) const -> void;
    virtual auto DoHandlePastDue(EventType&&) const -> void;
    virtual auto DoHandlePaymentFailed(EventType&&) const -> void;
    virtual auto DoHandleReady(EventType&&) const -> void;
    virtual auto DoHandleRevised(EventType&&) const -> void;
    virtual auto DoHandleUpdated(EventType&&) const -> void;
};

}  // namespace paddle::handlers
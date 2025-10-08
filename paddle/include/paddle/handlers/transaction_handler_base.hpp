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

    auto HandleEvent(const JSON& request_json, EventType&& event) const -> HandleResult;

    auto HandleBilled(EventType&& event) const -> HandleResult;
    auto HandleCanceled(EventType&& event) const -> HandleResult;
    auto HandleCompleted(EventType&& event) const -> HandleResult;
    auto HandleCreated(EventType&& event) const -> HandleResult;
    auto HandlePaid(EventType&& event) const -> HandleResult;
    auto HandlePastDue(EventType&& event) const -> HandleResult;
    auto HandlePaymentFailed(EventType&& event) const -> HandleResult;
    auto HandleReady(EventType&& event) const -> HandleResult;
    auto HandleRevised(EventType&& event) const -> HandleResult;
    auto HandleUpdated(EventType&& event) const -> HandleResult;

private:
    virtual auto DoHandleBilled(EventType&&) const -> HandleResult;
    virtual auto DoHandleCanceled(EventType&&) const -> HandleResult;
    virtual auto DoHandleCompleted(EventType&&) const -> HandleResult;
    virtual auto DoHandleCreated(EventType&&) const -> HandleResult;
    virtual auto DoHandlePaid(EventType&&) const -> HandleResult;
    virtual auto DoHandlePastDue(EventType&&) const -> HandleResult;
    virtual auto DoHandlePaymentFailed(EventType&&) const -> HandleResult;
    virtual auto DoHandleReady(EventType&&) const -> HandleResult;
    virtual auto DoHandleRevised(EventType&&) const -> HandleResult;
    virtual auto DoHandleUpdated(EventType&&) const -> HandleResult;
};

}  // namespace paddle::handlers
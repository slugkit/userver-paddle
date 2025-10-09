#pragma once

#include <paddle/types/events.hpp>
#include <paddle/types/formats.hpp>
#include <paddle/types/fwd.hpp>
#include <paddle/types/ids.hpp>

#include <userver/components/component_base.hpp>

namespace paddle::handlers {

class SubscriptionHandlerBase : public userver::components::ComponentBase {
public:
    using BaseType = userver::components::ComponentBase;
    using EventType = events::Event<subscriptions::Subscription>;
    constexpr static auto kEventCategory = events::EventCategory::kSubscription;

    using BaseType::BaseType;

    auto HandleEvent(const JSON& request_json, EventType&& event) const -> HandleResult;

    auto HandleActivated(EventType&& event) const -> HandleResult;
    auto HandleCanceled(EventType&& event) const -> HandleResult;
    auto HandleCreated(TransactionId&& transaction_id, EventType&& event) const -> HandleResult;
    auto HandleImported(EventType&& event) const -> HandleResult;
    auto HandlePastDue(EventType&& event) const -> HandleResult;
    auto HandlePaused(EventType&& event) const -> HandleResult;
    auto HandleResumed(EventType&& event) const -> HandleResult;
    auto HandleUpdated(EventType&& event) const -> HandleResult;
    auto HandleTrialing(EventType&& event) const -> HandleResult;

private:
    virtual auto DoHandleActivated(EventType&&) const -> HandleResult;
    virtual auto DoHandleCanceled(EventType&&) const -> HandleResult;
    virtual auto DoHandleCreated(TransactionId&&, EventType&&) const -> HandleResult;
    virtual auto DoHandleImported(EventType&&) const -> HandleResult;
    virtual auto DoHandlePastDue(EventType&&) const -> HandleResult;
    virtual auto DoHandlePaused(EventType&&) const -> HandleResult;
    virtual auto DoHandleResumed(EventType&&) const -> HandleResult;
    virtual auto DoHandleUpdated(EventType&&) const -> HandleResult;
    virtual auto DoHandleTrialing(EventType&&) const -> HandleResult;
};

}  // namespace paddle::handlers
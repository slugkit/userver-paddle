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

    auto HandleEvent(const JSON& request_json, EventType&& event) const -> void;

    auto HandleActivated(EventType&& event) const -> void;
    auto HandleCanceled(EventType&& event) const -> void;
    auto HandleCreated(TransactionId&& transaction_id, EventType&& event) const -> void;
    auto HandleImported(EventType&& event) const -> void;
    auto HandlePastDue(EventType&& event) const -> void;
    auto HandlePaused(EventType&& event) const -> void;
    auto HandleResumed(EventType&& event) const -> void;
    auto HandleUpdated(EventType&& event) const -> void;

private:
    virtual auto DoHandleActivated(EventType&&) const -> void;
    virtual auto DoHandleCanceled(EventType&&) const -> void;
    virtual auto DoHandleCreated(TransactionId&&, EventType&&) const -> void;
    virtual auto DoHandleImported(EventType&&) const -> void;
    virtual auto DoHandlePastDue(EventType&&) const -> void;
    virtual auto DoHandlePaused(EventType&&) const -> void;
    virtual auto DoHandleResumed(EventType&&) const -> void;
    virtual auto DoHandleUpdated(EventType&&) const -> void;
};

}  // namespace paddle::handlers
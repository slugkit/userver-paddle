#pragma once

#include <paddle/types/events.hpp>
#include <paddle/types/formats.hpp>
#include <paddle/types/fwd.hpp>
#include <paddle/types/price.hpp>

#include <userver/components/component_base.hpp>
#include <userver/utils/fast_pimpl.hpp>

namespace paddle::handlers {

class PriceHandlerBase : public userver::components::ComponentBase {
public:
    using BaseType = userver::components::ComponentBase;
    using PriceType = prices::JsonPrice;
    using EventType = events::Event<PriceType>;
    constexpr static auto kEventCategory = events::EventCategory::kPrice;

    PriceHandlerBase(
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    );
    ~PriceHandlerBase() override;

    static auto GetStaticConfigSchema() -> userver::yaml_config::Schema;

    auto HandleEvent(const JSON& request_json, EventType&& event) const -> HandleResult;

    auto HandleCreated(EventType&& event) const -> HandleResult;
    auto HandleImported(EventType&& event) const -> HandleResult;
    auto HandleUpdated(EventType&& event) const -> HandleResult;

private:
    virtual auto DoHandleCreated(EventType&&) const -> HandleResult;
    virtual auto DoHandleImported(EventType&&) const -> HandleResult;
    virtual auto DoHandleUpdated(EventType&&) const -> HandleResult;

private:
    constexpr static auto kImplSize = 48UL;
    constexpr static auto kImplAlign = 8UL;
    struct Impl;
    userver::utils::FastPimpl<Impl, kImplSize, kImplAlign> impl_;
};

}  // namespace paddle::handlers
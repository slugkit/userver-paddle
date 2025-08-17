#pragma once

#include <paddle/types/events.hpp>

#include <userver/components/component_base.hpp>

#include <userver/utils/fast_pimpl.hpp>

#include <functional>

namespace paddle::components {

/// @brief Component fetches events from Paddle and replays them to
/// the local handlers
class EventReplayController : public userver::components::ComponentBase {
public:
    using BaseType = userver::components::ComponentBase;
    using ReplayInfoCallback = std::function<void(const events::Event<JSON>& event)>;

    EventReplayController(
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    );
    ~EventReplayController() override;

    static auto GetStaticConfigSchema() -> userver::yaml_config::Schema;

    /// @brief Replay a single event to the local handlers
    void Replay(events::Event<JSON>&& event) const;

    void ReplaySince(std::string_view cursor, ReplayInfoCallback callback = nullptr) const;

private:
    constexpr static auto kImplSize = 88UL;
    constexpr static auto kImplAlign = 8UL;
    struct Impl;
    userver::utils::FastPimpl<Impl, kImplSize, kImplAlign> impl_;
};

}  // namespace paddle::components
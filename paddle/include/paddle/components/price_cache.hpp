#pragma once

#include <paddle/types/fwd.hpp>
#include <paddle/types/ids.hpp>

#include <userver/cache/caching_component_base.hpp>
#include <userver/server/http/http_request.hpp>
#include <userver/utils/fast_pimpl.hpp>

#include <string>
#include <unordered_map>

namespace paddle::components {

class PriceCache final : public userver::components::CachingComponentBase<
                             std::unordered_map<PriceId, prices::Price>> {
public:
  using BaseType = userver::components::CachingComponentBase<
      std::unordered_map<PriceId, prices::Price>>;

public:
  static constexpr auto kName = "paddle-price-cache";

  PriceCache(const userver::components::ComponentConfig &config,
             const userver::components::ComponentContext &context);
  ~PriceCache() override;

  static auto GetStaticConfigSchema() -> userver::yaml_config::Schema;

  auto AddPrice(const prices::Price &price) -> void;
  auto UpdatePrice(const prices::Price &price) -> void;
  auto RemovePrice(const prices::Price &price) -> void;

private:
  auto
  Update(userver::cache::UpdateType type,
         const std::chrono::system_clock::time_point &last_update,
         const std::chrono::system_clock::time_point &now,
         userver::cache::UpdateStatisticsScope &stats_scope) -> void override;

private:
  constexpr static auto kImplSize = 8UL;
  constexpr static auto kImplAlign = 8UL;
  struct Impl;
  userver::utils::FastPimpl<Impl, kImplSize, kImplAlign> impl_;
};

} // namespace paddle::components
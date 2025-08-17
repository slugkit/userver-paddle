#include <paddle/components/price_cache.hpp>

#include <paddle/components/client.hpp>
#include <paddle/types/price.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/logging/log.hpp>
#include <userver/tracing/span.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include <string>
#include <unordered_map>

namespace paddle::components {

namespace {
constexpr static auto kCopyStage = "copy";
constexpr static auto kFetchStage = "fetch";
constexpr static auto kParseStage = "parse";
} // namespace

struct PriceCache::Impl {
  using DataType = PriceCache::DataType;
  Client &client;

  Impl(const userver::components::ComponentConfig &config,
       const userver::components::ComponentContext &context)
      : client(context.FindComponent<Client>(
            config["client_name"].As<std::string>("paddle-client"))) {}

  std::unique_ptr<DataType>
  FetchPrices(userver::cache::UpdateStatisticsScope &stats_scope) const {
    namespace tracing = userver::tracing;
    auto scope =
        tracing::Span::CurrentSpan().CreateScopeTime(std::string{kCopyStage});
    auto data_cache = std::make_unique<DataType>();

    scope.Reset(std::string{kFetchStage});

    auto prices = client.GetAllPrices();
    LOG_INFO() << "Fetched " << prices.size() << " prices";
    scope.Reset(std::string{kParseStage});
    for (auto &price : prices) {
      auto id = price.id;
      data_cache->insert_or_assign(std::move(id), std::move(price));
    }
    LOG_INFO() << "Price cache updated with " << data_cache->size()
               << " prices";
    // Update the cache
    auto final_size = data_cache->size();
    stats_scope.IncreaseDocumentsReadCount(prices.size());
    stats_scope.Finish(final_size);
    return data_cache;
  }
};

PriceCache::PriceCache(const userver::components::ComponentConfig &config,
                       const userver::components::ComponentContext &context)
    : BaseType{config, context}, impl_{config, context} {
  StartPeriodicUpdates();
}

PriceCache::~PriceCache() { StopPeriodicUpdates(); }

auto PriceCache::GetStaticConfigSchema() -> userver::yaml_config::Schema {
  return userver::yaml_config::MergeSchemas<BaseType>(R"(
type: object
description: Price cache component
additionalProperties: false
properties:
    client_name:
        type: string
        description: Component name for Paddle client (paddle-client by default)
    )");
}

auto PriceCache::Update(
    [[maybe_unused]] userver::cache::UpdateType type,
    [[maybe_unused]] const std::chrono::system_clock::time_point &last_update,
    [[maybe_unused]] const std::chrono::system_clock::time_point &now,
    userver::cache::UpdateStatisticsScope &stats_scope) -> void {
  auto data_cache = impl_->FetchPrices(stats_scope);
  this->Set(std::move(data_cache));
}

auto PriceCache::AddPrice(const prices::Price &price) -> void {
  auto existing_data = this->Get();
  auto new_data = std::make_unique<DataType>(*existing_data);
  auto id = price.id;
  new_data->insert_or_assign(std::move(id), price);
  this->Set(std::move(new_data));
}

auto PriceCache::UpdatePrice(const prices::Price &price) -> void {
  auto existing_data = this->Get();
  auto new_data = std::make_unique<DataType>(*existing_data);
  auto id = price.id;
  new_data->insert_or_assign(std::move(id), price);
  this->Set(std::move(new_data));
}

} // namespace paddle::components

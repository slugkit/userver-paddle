#include <paddle/components/product_cache.hpp>

#include <paddle/components/client.hpp>
#include <paddle/types/product.hpp>

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

struct ProductCache::Impl {
  using DataType = ProductCache::DataType;
  Client &client;

  Impl(const userver::components::ComponentConfig &config,
       const userver::components::ComponentContext &context)
      : client(context.FindComponent<Client>(
            config["client_name"].As<std::string>("paddle-client"))) {}

  std::unique_ptr<DataType>
  FetchProducts(userver::cache::UpdateStatisticsScope &stats_scope) const {
    namespace tracing = userver::tracing;
    auto scope =
        tracing::Span::CurrentSpan().CreateScopeTime(std::string{kCopyStage});
    auto data_cache = std::make_unique<DataType>();

    scope.Reset(std::string{kFetchStage});

    auto products = client.GetAllProducts();
    LOG_INFO() << "Fetched " << products.size() << " products";
    scope.Reset(std::string{kParseStage});
    for (auto &product : products) {
      auto id = product.id;
      data_cache->insert_or_assign(std::move(id), std::move(product));
    }
    LOG_INFO() << "Product cache updated with " << data_cache->size()
               << " products";
    // Update the cache
    auto final_size = data_cache->size();
    stats_scope.IncreaseDocumentsReadCount(products.size());
    stats_scope.Finish(final_size);
    return data_cache;
  }
};

ProductCache::ProductCache(const userver::components::ComponentConfig &config,
                           const userver::components::ComponentContext &context)
    : BaseType(config, context), impl_(config, context) {}

ProductCache::~ProductCache() = default;

auto ProductCache::GetStaticConfigSchema() -> userver::yaml_config::Schema {
  return userver::yaml_config::MergeSchemas<BaseType>(R"(
type: object
description: Product cache component
additionalProperties: false
properties:
    client_name:
        type: string
        description: Component name for Paddle client (paddle-client by default)
    )");
}

auto ProductCache::Update(
    [[maybe_unused]] userver::cache::UpdateType type,
    [[maybe_unused]] const std::chrono::system_clock::time_point &last_update,
    [[maybe_unused]] const std::chrono::system_clock::time_point &now,
    userver::cache::UpdateStatisticsScope &stats_scope) -> void {
  auto data_cache = impl_->FetchProducts(stats_scope);
  this->Set(std::move(data_cache));
}

} // namespace paddle::components
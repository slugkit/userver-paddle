#include <paddle/components/price_cache.hpp>

#include <paddle/components/client.hpp>
#include <paddle/components/scope_names.hpp>
#include <paddle/types/price.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/logging/log.hpp>
#include <userver/tracing/span.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include <string>
#include <unordered_map>

namespace paddle::components::impl {

auto PriceCacheBase::FetchPrices(userver::cache::UpdateStatisticsScope& stats_scope, PriceListCallback callback)
    -> void {
    std::string cursor;
    bool has_more = true;

    auto scope = userver::tracing::Span::CurrentSpan().CreateScopeTime(std::string{scope_names::kFetchStage});
    auto doc_count = 0;
    while (has_more) {
        scope.Reset(std::string{scope_names::kFetchStage});
        auto response = client_.GetPrices(cursor, per_page_);
        auto prices = std::get<0>(response);
        cursor = std::get<1>(response);
        has_more = std::get<2>(response);
        doc_count += prices.size();
        scope.Reset(std::string{scope_names::kParseStage});
        callback(std::move(prices));
    }
    LOG_INFO() << "Fetched " << doc_count << " prices";
    stats_scope.IncreaseDocumentsReadCount(doc_count);
}

}  // namespace paddle::components::impl

#pragma once

#include <userver/cache/caching_component_base.hpp>
#include <userver/server/http/http_request.hpp>
#include <userver/utils/fast_pimpl.hpp>

#include <string>
#include <unordered_map>

namespace paddle::components {

/// @brief Cache for webhook secrets
///
/// The cache is used to store webhook secrets for each webhook.
/// The cache is updated from paddle API.
/// The cache is used to validate webhook requests.
class WebhookSecretCache final
    : public userver::components::CachingComponentBase<std::unordered_map<std::string, std::string>> {
public:
    using BaseType = userver::components::CachingComponentBase<std::unordered_map<std::string, std::string>>;

public:
    WebhookSecretCache(
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    );
    ~WebhookSecretCache() override;

    static auto GetStaticConfigSchema() -> userver::yaml_config::Schema;

    auto ValidateSignature(const userver::server::http::HttpRequest& request) const -> bool;

private:
    auto Update(
        userver::cache::UpdateType type,
        const std::chrono::system_clock::time_point& last_update,
        const std::chrono::system_clock::time_point& now,
        userver::cache::UpdateStatisticsScope& stats_scope
    ) -> void override;

private:
    constexpr static auto kImplSize = 96UL;
    constexpr static auto kImplAlign = 8UL;
    struct Impl;
    userver::utils::FastPimpl<Impl, kImplSize, kImplAlign> impl_;
};
}  // namespace paddle::components
#include <paddle/components/webhook_secret_cache.hpp>

#include <paddle/auth/signature.hpp>
#include <paddle/components/client.hpp>

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
}  // namespace

struct WebhookSecretCache::Impl {
    using DataType = WebhookSecretCache::DataType;

    Client& client;
    std::string webhook_host;
    std::int32_t max_signature_age_seconds;

    Impl(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
        : client(context.FindComponent<Client>(config["client_name"].As<std::string>("paddle-client")))
        , webhook_host(config["webhook_host"].As<std::string>())
        , max_signature_age_seconds(config["max_signature_age_seconds"].As<std::int32_t>(60)) {
    }

    std::unique_ptr<DataType> FetchSecrets(userver::cache::UpdateStatisticsScope& stats_scope) const {
        namespace tracing = userver::tracing;
        // Get all notification settings from the client
        auto scope = tracing::Span::CurrentSpan().CreateScopeTime(std::string{kCopyStage});
        auto data_cache = std::make_unique<DataType>();

        scope.Reset(std::string{kFetchStage});

        // Get all notification settings from the client
        auto notification_settings = client.GetAllNotificationSettings();
        LOG_INFO() << "Fetched " << notification_settings.size() << " notification settings";
        scope.Reset(std::string{kParseStage});
        // First we build a map of webhook destinations to their secret keys
        // Also we filter out the destinations that do not match the webhook host
        for (const auto& notification_setting : notification_settings) {
            if (notification_setting.type != NotificationSettingType::kUrl) {
                continue;
            }
            if (!notification_setting.active) {
                continue;
            }
            auto pos = notification_setting.destination.find(webhook_host);
            if (pos == std::string::npos) {
                continue;
            }
            auto webhook_path = notification_setting.destination.substr(pos + webhook_host.size());
            LOG_INFO() << "Adding webhook secret: '" << webhook_path << "'";
            data_cache->insert_or_assign(std::move(webhook_path), std::move(notification_setting.endpoint_secret_key));
        }
        LOG_INFO() << "Webhook secret cache updated with " << data_cache->size() << " webhooks";
        // Update the cache
        auto final_size = data_cache->size();
        stats_scope.IncreaseDocumentsReadCount(notification_settings.size());
        stats_scope.Finish(final_size);
        return data_cache;
    }
};

WebhookSecretCache::WebhookSecretCache(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
)
    : BaseType{config, context}
    , impl_{config, context} {
    StartPeriodicUpdates();
}

WebhookSecretCache::~WebhookSecretCache() {
    StopPeriodicUpdates();
}

auto WebhookSecretCache::GetStaticConfigSchema() -> userver::yaml_config::Schema {
    return userver::yaml_config::MergeSchemas<BaseType>(R"(
type: object
description: Webhook secret cache component
additionalProperties: false
properties:
    client_name:
        type: string
        description: Component name for Paddle client (paddle-client by default)
    webhook_host:
        type: string
        description: Hostname of the webhook server
    max_signature_age_seconds:
        type: integer
        description: Maximum age of the signature in seconds
   )");
}

auto WebhookSecretCache::Update(
    [[maybe_unused]] userver::cache::UpdateType type,
    [[maybe_unused]] const std::chrono::system_clock::time_point& last_update,
    [[maybe_unused]] const std::chrono::system_clock::time_point& now,
    userver::cache::UpdateStatisticsScope& stats_scope
) -> void {
    auto data_cache = impl_->FetchSecrets(stats_scope);
    this->Set(std::move(data_cache));
}

auto WebhookSecretCache::ValidateSignature(const userver::server::http::HttpRequest& request) const -> bool {
    auto path = request.GetRequestPath();
    auto secret_cache = this->GetUnsafe();
    auto secret = secret_cache->find(path);
    if (secret == secret_cache->end()) {
        LOG_WARNING() << "No secret found for path: " << path;
        return false;
    }
    auto signature = request.GetHeader("Paddle-Signature");
    if (signature.empty()) {
        LOG_WARNING() << "No signature found in request";
        return false;
    }
    auto payload = request.RequestBody();
    return VerifySignature(secret->second, signature, payload, impl_->max_signature_age_seconds);
}

}  // namespace paddle::components
#pragma once

#include <paddle/types/notification_settings.hpp>
// #include <paddle/types/price.hpp>
// #include <paddle/types/product.hpp>
// #include <paddle/types/subscriptions.hpp>
#include <paddle/types/fwd.hpp>

#include <userver/clients/http/client.hpp>
#include <userver/components/component_base.hpp>
#include <userver/utils/fast_pimpl.hpp>

#include <cstdint>
#include <tuple>
#include <vector>

namespace paddle::components {

/// @brief Client for Paddle API
///
/// The client is used to wrap the Paddle API calls.
/// Configuration:
/// - base_url: base URL of the Paddle API
/// - api_key: API key for the Paddle API
/// - api_version: API version to use, default is "1"
/// - webhook_host: Hostname of the webhook server
/// - webhooks: Webhooks mapping, the key is the handler name, the value is the webhook path
class Client final : public userver::components::ComponentBase {
public:
    static constexpr std::int32_t kDefaultPerPage = 200;

    static constexpr std::string_view kName = "paddle-client";

    Client(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context);
    ~Client();

    static auto GetStaticConfigSchema() -> userver::yaml_config::Schema;

    [[nodiscard]] auto GetAllNotificationSettings() const -> std::vector<NotificationSetting>;
    [[nodiscard]] auto GetNotificationSettings(std::string_view cursor, std::int32_t per_page = kDefaultPerPage) const
        -> ResponseWithCursor<NotificationSetting>;

    [[nodiscard]] auto GetAllEvents() const -> std::vector<events::Event<JSON>>;
    [[nodiscard]] auto GetEvents(std::string_view cursor, std::int32_t per_page = kDefaultPerPage) const
        -> ResponseWithCursor<events::Event<JSON>>;

    [[nodiscard]] auto GetAllProducts() const -> std::vector<products::Product>;
    [[nodiscard]] auto GetProducts(std::string_view cursor, std::int32_t per_page = kDefaultPerPage) const
        -> ResponseWithCursor<products::Product>;

    [[nodiscard]] auto GetAllPrices() const -> std::vector<prices::Price>;
    [[nodiscard]] auto GetPrices(std::string_view cursor, std::int32_t per_page = kDefaultPerPage) const
        -> ResponseWithCursor<prices::Price>;

    [[nodiscard]] auto GetAllSubscriptions() const -> std::vector<subscriptions::Subscription>;
    [[nodiscard]] auto GetSubscriptions(std::string_view cursor, std::int32_t per_page = kDefaultPerPage) const
        -> ResponseWithCursor<subscriptions::Subscription>;

private:
    constexpr static auto kImplSize = 104UL;
    constexpr static auto kImplAlign = 8UL;
    struct Impl;
    userver::utils::FastPimpl<Impl, kImplSize, kImplAlign> impl_;
};

}  // namespace paddle::components
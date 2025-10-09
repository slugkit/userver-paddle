#pragma once

#include <paddle/types/customers.hpp>
#include <paddle/types/fwd.hpp>
#include <paddle/types/notification_settings.hpp>
#include <paddle/types/price.hpp>
#include <paddle/types/price_preview.hpp>
#include <paddle/types/product.hpp>
#include <paddle/types/subscriptions.hpp>

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
/// - webhooks: Webhooks mapping, the key is the handler name, the value is the
/// webhook path
class Client final : public userver::components::ComponentBase {
public:
    static constexpr std::int32_t kDefaultPerPage = 200;

    static constexpr std::string_view kName = "paddle-client";

    Client(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context);
    ~Client();

    static auto GetStaticConfigSchema() -> userver::yaml_config::Schema;

    //@{
    /// @name Notification settings
    [[nodiscard]] auto GetAllNotificationSettings() const -> std::vector<NotificationSetting>;
    [[nodiscard]] auto GetNotificationSettings(std::string_view cursor, std::int32_t per_page = kDefaultPerPage) const
        -> ResponseWithCursor<NotificationSetting>;
    //@}

    //@{
    /// @name Events
    [[nodiscard]] auto GetAllEvents() const -> std::vector<events::Event<JSON>>;
    [[nodiscard]] auto GetEvents(std::string_view cursor, std::int32_t per_page = kDefaultPerPage) const
        -> ResponseWithCursor<events::Event<JSON>>;
    //@}

    //@{
    /// @name Products
    [[nodiscard]] auto GetAllProducts() const -> std::vector<products::JsonProduct>;
    [[nodiscard]] auto GetProducts(std::string_view cursor, std::int32_t per_page = kDefaultPerPage) const
        -> ResponseWithCursor<products::JsonProduct>;
    //@}

    //@{
    /// @name Prices
    [[nodiscard]] auto GetAllPrices() const -> std::vector<prices::JsonPrice>;
    [[nodiscard]] auto GetPrices(std::string_view cursor, std::int32_t per_page = kDefaultPerPage) const
        -> ResponseWithCursor<prices::JsonPrice>;
    //@{
    /// @name Price preview
    [[nodiscard]] auto GetPricePreview(const prices::PricePreviewRequest& request) const -> prices::JsonPricePreview;
    template <typename ProductData, typename PriceData>
    [[nodiscard]] auto GetPricePreview(const prices::PricePreviewRequest& request
    ) const -> prices::PricePreviewTemplate<ProductData, PriceData> {
        auto response = GetPricePreview(request);
        return Convert(response, convert::To<prices::PricePreviewTemplate<ProductData, PriceData>>{});
    }
    //@}
    //@}

    //@{
    /// @name Customers
    [[nodiscard]] auto GetAllCustomers() const -> std::vector<customers::Customer>;
    [[nodiscard]] auto GetCustomers(std::string_view cursor, std::int32_t per_page = kDefaultPerPage) const
        -> ResponseWithCursor<customers::Customer>;
    [[nodiscard]] auto UpdateCustomer(
        const CustomerId& customer_id,
        const customers::CustomerUpdateRequest& customer_update_request
    ) const -> customers::Customer;
    //@}

    //@{
    /// @name Subscriptions
    [[nodiscard]] auto GetAllSubscriptions() const -> std::vector<subscriptions::Subscription>;
    [[nodiscard]] auto GetSubscriptions(std::string_view cursor, std::int32_t per_page = kDefaultPerPage) const
        -> ResponseWithCursor<subscriptions::Subscription>;
    //@}

private:
    constexpr static auto kImplSize = 104UL;
    constexpr static auto kImplAlign = 8UL;
    struct Impl;
    userver::utils::FastPimpl<Impl, kImplSize, kImplAlign> impl_;
};

}  // namespace paddle::components
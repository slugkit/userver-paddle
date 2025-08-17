#pragma once

#include <userver/components/component_fwd.hpp>
#include <userver/yaml_config/schema.hpp>

#include <string_view>

namespace paddle::handlers {

// Forward declarations
class AddressHandlerBase;
class ApiKeyHandlerBase;
class BusinessHandlerBase;
class ClientTokenHandlerBase;
class CustomerHandlerBase;
class PaymentMethodHandlerBase;
class PriceHandlerBase;
class ProductHandlerBase;
class SubscriptionHandlerBase;
class TransactionHandlerBase;

/// @brief Collection of event handlers
struct Handlers {
    AddressHandlerBase* address_handler = nullptr;
    ApiKeyHandlerBase* api_key_handler = nullptr;
    BusinessHandlerBase* business_handler = nullptr;
    ClientTokenHandlerBase* client_token_handler = nullptr;
    CustomerHandlerBase* customer_handler = nullptr;
    PaymentMethodHandlerBase* payment_method_handler = nullptr;
    PriceHandlerBase* price_handler = nullptr;
    ProductHandlerBase* product_handler = nullptr;
    SubscriptionHandlerBase* subscription_handler = nullptr;
    TransactionHandlerBase* transaction_handler = nullptr;

    Handlers(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context);

    [[nodiscard]] auto IsEmpty() const -> bool {
        return address_handler == nullptr && api_key_handler == nullptr && business_handler == nullptr &&
               client_token_handler == nullptr && customer_handler == nullptr && payment_method_handler == nullptr &&
               price_handler == nullptr && product_handler == nullptr && subscription_handler == nullptr &&
               transaction_handler == nullptr;
    }

    static auto GetHanderNames() -> std::string_view;
};

}  // namespace paddle::handlers
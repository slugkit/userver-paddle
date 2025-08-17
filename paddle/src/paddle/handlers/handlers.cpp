#include <paddle/handlers/handlers.hpp>

#include <paddle/handlers/address_handler_base.hpp>
#include <paddle/handlers/api_key_handler_base.hpp>
#include <paddle/handlers/business_handler_base.hpp>
#include <paddle/handlers/client_token_handler_base.hpp>
#include <paddle/handlers/customer_handler_base.hpp>
#include <paddle/handlers/payment_method_handler_base.hpp>
#include <paddle/handlers/price_handler_base.hpp>
#include <paddle/handlers/product_handler_base.hpp>
#include <paddle/handlers/subscription_handler_base.hpp>
#include <paddle/handlers/transaction_handler_base.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>

#include <userver/yaml_config/merge_schemas.hpp>

namespace paddle::handlers {

Handlers::Handlers(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
) {
    auto transaction_handler_name = config["transactions"].As<std::string>("");
    if (!transaction_handler_name.empty()) {
        transaction_handler = &context.FindComponent<TransactionHandlerBase>(transaction_handler_name);
    }
    auto subscription_handler_name = config["subscriptions"].As<std::string>("");
    if (!subscription_handler_name.empty()) {
        subscription_handler = &context.FindComponent<SubscriptionHandlerBase>(subscription_handler_name);
    }
    auto customer_handler_name = config["customers"].As<std::string>("");
    if (!customer_handler_name.empty()) {
        customer_handler = &context.FindComponent<CustomerHandlerBase>(customer_handler_name);
    }
    auto payment_method_handler_name = config["payment_methods"].As<std::string>("");
    if (!payment_method_handler_name.empty()) {
        payment_method_handler = &context.FindComponent<PaymentMethodHandlerBase>(payment_method_handler_name);
    }
    auto price_handler_name = config["prices"].As<std::string>("");
    if (!price_handler_name.empty()) {
        price_handler = &context.FindComponent<PriceHandlerBase>(price_handler_name);
    }
    auto product_handler_name = config["products"].As<std::string>("");
    if (!product_handler_name.empty()) {
        product_handler = &context.FindComponent<ProductHandlerBase>(product_handler_name);
    }
    auto address_handler_name = config["addresses"].As<std::string>("");
    if (!address_handler_name.empty()) {
        address_handler = &context.FindComponent<AddressHandlerBase>(address_handler_name);
    }
    auto business_handler_name = config["businesses"].As<std::string>("");
    if (!business_handler_name.empty()) {
        business_handler = &context.FindComponent<BusinessHandlerBase>(business_handler_name);
    }
    auto api_key_handler_name = config["api_keys"].As<std::string>("");
    if (!api_key_handler_name.empty()) {
        api_key_handler = &context.FindComponent<ApiKeyHandlerBase>(api_key_handler_name);
    }
    auto client_token_handler_name = config["client_tokens"].As<std::string>("");
    if (!client_token_handler_name.empty()) {
        client_token_handler = &context.FindComponent<ClientTokenHandlerBase>(client_token_handler_name);
    }
}

auto Handlers::GetHanderNames() -> std::string_view {
    return R"(
    transactions:
        type: string
        description: Component name for transaction event handlers
    subscriptions:
        type: string
        description: Component name for subscription event handlers
    customers:
        type: string
        description: Component name for customer event handlers
    payment_methods:
        type: string
        description: Component name for payment method event handlers
    prices:
        type: string
        description: Component name for price event handlers
    products:
        type: string
        description: Component name for product event handlers
    addresses:
        type: string
        description: Component name for address event handlers
    businesses:
        type: string
        description: Component name for business event handlers
    api_keys:
        type: string
        description: Component name for API key event handlers
    client_tokens:
        type: string
        description: Component name for client token event handlers
    )";
}

}  // namespace paddle::handlers
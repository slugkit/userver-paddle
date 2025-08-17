#include <paddle/components/webhook_secret_cache.hpp>
#include <paddle/components/client.hpp>

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
#include <paddle/handlers/webhook_handler.hpp>

#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

int main(int argc, char* argv[]) {
    auto component_list = userver::components::MinimalServerComponentList()
        .Append<userver::server::handlers::Ping>()
        .Append<userver::components::HttpClient>()
        .Append<userver::server::handlers::TestsControl>()

        .Append<paddle::components::Client>()
        .Append<paddle::components::WebhookSecretCache>("webhook-secret-cache")

        .Append<paddle::handlers::AddressHandlerBase>("handler-address")
        .Append<paddle::handlers::ApiKeyHandlerBase>("handler-api-key")
        .Append<paddle::handlers::BusinessHandlerBase>("handler-business")
        .Append<paddle::handlers::ClientTokenHandlerBase>("handler-client-token")
        .Append<paddle::handlers::CustomerHandlerBase>("handler-customer")
        .Append<paddle::handlers::PaymentMethodHandlerBase>("handler-payment-method")
        .Append<paddle::handlers::PriceHandlerBase>("handler-price")
        .Append<paddle::handlers::ProductHandlerBase>("handler-product")
        .Append<paddle::handlers::SubscriptionHandlerBase>("handler-subscription")
        .Append<paddle::handlers::TransactionHandlerBase>("handler-transaction")

        .Append<paddle::handlers::WebhookHandler>("handler-webhook")
    ;


    return userver::utils::DaemonMain(argc, argv, component_list);
}
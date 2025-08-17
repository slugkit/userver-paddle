#include <paddle/types/subscriptions.hpp>

#include <userver/utest/utest.hpp>

namespace paddle {

namespace {

const auto kTrialingSubscription = R"({
    "id": "sub_01k2jjkzv4h5te6zw46gfnrxnw",
    "status": "trialing",
    "customer_id": "ctm_01k2jj0xbzdpzbgz0vqv1e0x5e",
    "address_id": "add_01k2jjgd1xzzwtarjsw7qw2by3",
    "business_id": null,
    "currency_code": "EUR",
    "created_at": "2025-08-13T20:40:49.764Z",
    "updated_at": "2025-08-13T20:40:49.764Z",
    "started_at": "2025-08-13T20:40:49.761Z",
    "first_billed_at": null,
    "next_billed_at": "2025-08-20T20:40:49.761Z",
    "paused_at": null,
    "canceled_at": null,
    "collection_mode": "automatic",
    "billing_details": null,
    "current_billing_period": {
        "starts_at": "2025-08-13T20:40:49.761Z",
        "ends_at": "2025-08-20T20:40:49.761Z"
    },
    "billing_cycle": {
        "frequency": 1,
        "interval": "month"
    },
    "scheduled_change": null,
    "items": [
        {
            "status": "trialing",
            "quantity": 1,
            "recurring": true,
            "created_at": "2025-08-13T20:40:49.764Z",
            "updated_at": "2025-08-13T20:40:49.764Z",
            "previously_billed_at": null,
            "next_billed_at": "2025-08-20T20:40:49.761Z",
            "trial_dates": {
                "starts_at": "2025-08-13T20:40:49.761Z",
                "ends_at": "2025-08-20T20:40:49.761Z"
            },
            "price": {
                "id": "pri_01k2jg2k8f8dhy4yvvtq2j2nh1",
                "product_id": "pro_01k2jfwwcvmscf97tnxxa0v81t",
                "type": "standard",
                "description": "saas-developer-monthly",
                "name": "SlugKit SaaS Indy Monthly Subscription",
                "tax_mode": "internal",
                "billing_cycle": {
                    "frequency": 1,
                    "interval": "month"
                },
                "trial_period": {
                    "frequency": 7,
                    "interval": "day"
                },
                "unit_price": {
                    "amount": "500",
                    "currency_code": "USD"
                },
                "unit_price_overrides": [],
                "custom_data": {
                    "slug": "saas-developer"
                },
                "status": "active",
                "quantity": {
                    "minimum": 1,
                    "maximum": 1
                },
                "import_meta": null,
                "created_at": "2025-08-13T19:56:22.671769Z",
                "updated_at": "2025-08-13T19:57:57.766584Z"
            },
            "product": {
                "id": "pro_01k2jfwwcvmscf97tnxxa0v81t",
                "name": "SaaS Indy",
                "type": "standard",
                "tax_category": "saas",
                "description": "SlugKit subscription plan for individual developers",
                "image_url": "https://dev.slugkit.dev/favicon.png",
                "custom_data": {
                    "slug": "saas-developer"
                },
                "status": "active",
                "created_at": "2025-08-13T19:53:15.419Z",
                "updated_at": "2025-08-13T20:15:09.484Z"
            }
        }
    ],
    "custom_data": null,
    "management_urls": {
        "update_payment_method": "https://sandbox-customer-portal.paddle.com/cpl_01k2jcck84zd4db044r7r8tvny?action=update_subscription_payment_method&subscription_id=sub_01k2jjkzv4h5te6zw46gfnrxnw&token=pga_eyJhbGciOiJFZERTQSIsImtpZCI6Imp3a18wMWhkazBuOHF3OG55NTJ5cGNocGNhazA1ayIsInR5cCI6IkpXVCJ9.eyJpZCI6InBnYV8wMWsyam03enJ4eXZyZmp6dzc2NDdreWticCIsInNlbGxlci1pZCI6IjM2MjEwIiwidHlwZSI6InN0YW5kYXJkIiwidmVyc2lvbiI6IjEiLCJ1c2FnZSI6ImN1c3RvbWVyLXBvcnRhbC11cmwiLCJzY29wZSI6ImN1c3RvbWVyLmFkanVzdG1lbnQucmVhZCBjdXN0b21lci5jaGVja291dC5jcmVhdGUgY3VzdG9tZXIuY2hlY2tvdXQucmVhZCBjdXN0b21lci5jdXN0b21lci5yZWFkIGN1c3RvbWVyLmN1c3RvbWVyLnVwZGF0ZSBjdXN0b21lci5jdXN0b21lci1hZGRyZXNzLnJlYWQgY3VzdG9tZXIuY3VzdG9tZXItYWRkcmVzcy51cGRhdGUgY3VzdG9tZXIuY3VzdG9tZXItYnVzaW5lc3MucmVhZCBjdXN0b21lci5jdXN0b21lci1idXNpbmVzcy5jcmVhdGUgY3VzdG9tZXIuY3VzdG9tZXItYnVzaW5lc3MudXBkYXRlIGN1c3RvbWVyLmN1c3RvbWVyLXBheW1lbnQtbWV0aG9kLnJlYWQgY3VzdG9tZXIuY3VzdG9tZXItcGF5bWVudC1tZXRob2QuZGVsZXRlIGN1c3RvbWVyLmludm9pY2UucmVhZCBjdXN0b21lci5zdWJzY3JpcHRpb24tY2FuY2VsLmNyZWF0ZSBjdXN0b21lci5zdWJzY3JpcHRpb24tY29uc2VudC1yZXF1aXJlbWVudC1ncmFudC5jcmVhdGUgY3VzdG9tZXIuc3Vic2NyaXB0aW9uLWNvbnNlbnQtcmVxdWlyZW1lbnQucmVhZCBjdXN0b21lci5zdWJzY3JpcHRpb24ucmVhZCBjdXN0b21lci5zdWJzY3JpcHRpb24udXBkYXRlIGN1c3RvbWVyLnRyYW5zYWN0aW9uLmNyZWF0ZSBjdXN0b21lci50cmFuc2FjdGlvbi5yZWFkIGN1c3RvbWVyLnRyYW5zYWN0aW9uLnVwZGF0ZSBjdXN0b21lci50cmFuc2FjdGlvbi5vcmlnaW4ucmVhZCIsImlzcyI6Imd1ZXN0YWNjZXNzLXNlcnZpY2UiLCJzdWIiOiJjdG1fMDFrMmpqMHhiemRwemJnejB2cXYxZTB4NWUiLCJleHAiOjE3NTUyMDU3NTMsImlhdCI6MTc1NTExOTM1M30.EI7WUsuJBEmazpP5NmL7y8CGgk_x3hUNZLgCbYbgmA92MKLbfybGBSXU23CQgovw2Xan6mIs3rjgwxuXJ84qDA",
        "cancel": "https://sandbox-customer-portal.paddle.com/cpl_01k2jcck84zd4db044r7r8tvny?action=cancel_subscription&subscription_id=sub_01k2jjkzv4h5te6zw46gfnrxnw&token=pga_eyJhbGciOiJFZERTQSIsImtpZCI6Imp3a18wMWhkazBuOHF3OG55NTJ5cGNocGNhazA1ayIsInR5cCI6IkpXVCJ9.eyJpZCI6InBnYV8wMWsyam03enJ4eXZyZmp6dzc2NDdreWticCIsInNlbGxlci1pZCI6IjM2MjEwIiwidHlwZSI6InN0YW5kYXJkIiwidmVyc2lvbiI6IjEiLCJ1c2FnZSI6ImN1c3RvbWVyLXBvcnRhbC11cmwiLCJzY29wZSI6ImN1c3RvbWVyLmFkanVzdG1lbnQucmVhZCBjdXN0b21lci5jaGVja291dC5jcmVhdGUgY3VzdG9tZXIuY2hlY2tvdXQucmVhZCBjdXN0b21lci5jdXN0b21lci5yZWFkIGN1c3RvbWVyLmN1c3RvbWVyLnVwZGF0ZSBjdXN0b21lci5jdXN0b21lci1hZGRyZXNzLnJlYWQgY3VzdG9tZXIuY3VzdG9tZXItYWRkcmVzcy51cGRhdGUgY3VzdG9tZXIuY3VzdG9tZXItYnVzaW5lc3MucmVhZCBjdXN0b21lci5jdXN0b21lci1idXNpbmVzcy5jcmVhdGUgY3VzdG9tZXIuY3VzdG9tZXItYnVzaW5lc3MudXBkYXRlIGN1c3RvbWVyLmN1c3RvbWVyLXBheW1lbnQtbWV0aG9kLnJlYWQgY3VzdG9tZXIuY3VzdG9tZXItcGF5bWVudC1tZXRob2QuZGVsZXRlIGN1c3RvbWVyLmludm9pY2UucmVhZCBjdXN0b21lci5zdWJzY3JpcHRpb24tY2FuY2VsLmNyZWF0ZSBjdXN0b21lci5zdWJzY3JpcHRpb24tY29uc2VudC1yZXF1aXJlbWVudC1ncmFudC5jcmVhdGUgY3VzdG9tZXIuc3Vic2NyaXB0aW9uLWNvbnNlbnQtcmVxdWlyZW1lbnQucmVhZCBjdXN0b21lci5zdWJzY3JpcHRpb24ucmVhZCBjdXN0b21lci5zdWJzY3JpcHRpb24udXBkYXRlIGN1c3RvbWVyLnRyYW5zYWN0aW9uLmNyZWF0ZSBjdXN0b21lci50cmFuc2FjdGlvbi5yZWFkIGN1c3RvbWVyLnRyYW5zYWN0aW9uLnVwZGF0ZSBjdXN0b21lci50cmFuc2FjdGlvbi5vcmlnaW4ucmVhZCIsImlzcyI6Imd1ZXN0YWNjZXNzLXNlcnZpY2UiLCJzdWIiOiJjdG1fMDFrMmpqMHhiemRwemJnejB2cXYxZTB4NWUiLCJleHAiOjE3NTUyMDU3NTMsImlhdCI6MTc1NTExOTM1M30.EI7WUsuJBEmazpP5NmL7y8CGgk_x3hUNZLgCbYbgmA92MKLbfybGBSXU23CQgovw2Xan6mIs3rjgwxuXJ84qDA"
    },
    "discount": null,
    "import_meta": null
})";

}  // namespace

UTEST(SubscriptionTest, Parse) {
    userver::formats::json::Value json = userver::formats::json::FromString(kTrialingSubscription);
    auto subscription = Parse(json, userver::formats::parse::To<subscriptions::Subscription>());
    EXPECT_EQ(subscription.id, SubscriptionId("sub_01k2jjkzv4h5te6zw46gfnrxnw"));
    EXPECT_EQ(subscription.status, SubscriptionStatus::kTrialing);
    EXPECT_EQ(subscription.customer_id, CustomerId("ctm_01k2jj0xbzdpzbgz0vqv1e0x5e"));
    EXPECT_EQ(subscription.address_id, AddressId("add_01k2jjgd1xzzwtarjsw7qw2by3"));
    EXPECT_EQ(subscription.business_id, std::nullopt);
    EXPECT_EQ(subscription.currency_code, money::CurrencyCode("EUR"));
}

}  // namespace paddle
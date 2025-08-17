#include <paddle/types/notification_settings.hpp>

#include <userver/utest/utest.hpp>

namespace paddle {

namespace {
const auto kNotificationSetting = R"({
    "id": "ntfset_01k2jjfqx34sdwsvrbj123wxx2",
    "description": "HookDeck",
    "type": "url",
    "destination": "https://hkdk.events/xieng6t7oceftj",
    "active": true,
    "api_version": 1,
    "include_sensitive_fields": true,
    "subscribed_events": [
        {
            "name": "transaction.billed",
            "description": "Occurs when a transaction is billed. Its status field changes to billed and billed_at is populated.",
            "group": "Transaction",
            "available_versions": [
                1
            ]
        },
        {
            "name": "transaction.canceled",
            "description": "Occurs when a transaction is canceled. Its status field changes to canceled.",
            "group": "Transaction",
            "available_versions": [
                1
            ]
        },
        {
            "name": "transaction.completed",
            "description": "Occurs when a transaction is completed. Its status field changes to completed.",
            "group": "Transaction",
            "available_versions": [
                1
            ]
        },
        {
            "name": "transaction.created",
            "description": "Occurs when a transaction is created.",
            "group": "Transaction",
            "available_versions": [
                1
            ]
        },
        {
            "name": "transaction.paid",
            "description": "Occurs when a transaction is paid. Its status field changes to paid.",
            "group": "Transaction",
            "available_versions": [
                1
            ]
        },
        {
            "name": "transaction.past_due",
            "description": "Occurs when a transaction becomes past due. Its status field changes to past_due.",
            "group": "Transaction",
            "available_versions": [
                1
            ]
        },
        {
            "name": "transaction.payment_failed",
            "description": "Occurs when a payment fails for a transaction. The payments array is updated with details of the payment attempt.",
            "group": "Transaction",
            "available_versions": [
                1
            ]
        },
        {
            "name": "transaction.ready",
            "description": "Occurs when a transaction is ready to be billed. Its status field changes to ready.",
            "group": "Transaction",
            "available_versions": [
                1
            ]
        },
        {
            "name": "transaction.updated",
            "description": "Occurs when a transaction is updated.",
            "group": "Transaction",
            "available_versions": [
                1
            ]
        },
        {
            "name": "transaction.revised",
            "description": "Occurs when a transaction is revised.",
            "group": "Transaction",
            "available_versions": [
                1
            ]
        },
        {
            "name": "subscription.activated",
            "description": "Occurs when a subscription becomes active. Its status field changes to active. This means any trial period has elapsed and Paddle has successfully billed the customer.",
            "group": "Subscription",
            "available_versions": [
                1
            ]
        },
        {
            "name": "subscription.canceled",
            "description": "Occurs when a subscription is canceled. Its status field changes to canceled.",
            "group": "Subscription",
            "available_versions": [
                1
            ]
        },
        {
            "name": "subscription.created",
            "description": "Occurs when a subscription is created. subscription.trialing or subscription.activated typically follow.",
            "group": "Subscription",
            "available_versions": [
                1
            ]
        },
        {
            "name": "subscription.imported",
            "description": "Occurs when a subscription is imported.",
            "group": "Subscription",
            "available_versions": [
                1
            ]
        },
        {
            "name": "subscription.past_due",
            "description": "Occurs when a subscription has an unpaid transaction. Its status changes to past_due.",
            "group": "Subscription",
            "available_versions": [
                1
            ]
        },
        {
            "name": "subscription.paused",
            "description": "Occurs when a subscription is paused. Its status field changes to paused.",
            "group": "Subscription",
            "available_versions": [
                1
            ]
        },
        {
            "name": "subscription.resumed",
            "description": "Occurs when a subscription is resumed after being paused. Its status field changes to active.",
            "group": "Subscription",
            "available_versions": [
                1
            ]
        },
        {
            "name": "subscription.trialing",
            "description": "Occurs when a subscription enters trial period.",
            "group": "Subscription",
            "available_versions": [
                1
            ]
        },
        {
            "name": "subscription.updated",
            "description": "Occurs when a subscription is updated.",
            "group": "Subscription",
            "available_versions": [
                1
            ]
        },
        {
            "name": "product.created",
            "description": "Occurs when a product is created.",
            "group": "Product",
            "available_versions": [
                1
            ]
        },
        {
            "name": "product.imported",
            "description": "Occurs when a product is imported.",
            "group": "Product",
            "available_versions": [
                1
            ]
        },
        {
            "name": "product.updated",
            "description": "Occurs when a product is updated.",
            "group": "Product",
            "available_versions": [
                1
            ]
        },
        {
            "name": "price.created",
            "description": "Occurs when a price is created.",
            "group": "Price",
            "available_versions": [
                1
            ]
        },
        {
            "name": "price.imported",
            "description": "Occurs when a price is imported.",
            "group": "Price",
            "available_versions": [
                1
            ]
        },
        {
            "name": "price.updated",
            "description": "Occurs when a price is updated.",
            "group": "Price",
            "available_versions": [
                1
            ]
        },
        {
            "name": "customer.created",
            "description": "Occurs when a customer is created.",
            "group": "Customer",
            "available_versions": [
                1
            ]
        },
        {
            "name": "customer.imported",
            "description": "Occurs when a customer is imported.",
            "group": "Customer",
            "available_versions": [
                1
            ]
        },
        {
            "name": "customer.updated",
            "description": "Occurs when a customer is updated.",
            "group": "Customer",
            "available_versions": [
                1
            ]
        },
        {
            "name": "payment_method.saved",
            "description": "Occurs when a customer saves a payment method to their account.",
            "group": "Payment Method",
            "available_versions": [
                1
            ]
        },
        {
            "name": "payment_method.deleted",
            "description": "Occurs when a customer removes a payment method from their account.",
            "group": "Payment Method",
            "available_versions": [
                1
            ]
        },
        {
            "name": "address.created",
            "description": "Occurs when an address is created.",
            "group": "Address",
            "available_versions": [
                1
            ]
        },
        {
            "name": "address.imported",
            "description": "Occurs when a address is imported.",
            "group": "Address",
            "available_versions": [
                1
            ]
        },
        {
            "name": "address.updated",
            "description": "Occurs when an address is updated.",
            "group": "Address",
            "available_versions": [
                1
            ]
        },
        {
            "name": "business.created",
            "description": "Occurs when a business is created.",
            "group": "Business",
            "available_versions": [
                1
            ]
        },
        {
            "name": "business.imported",
            "description": "Occurs when a business is imported.",
            "group": "Business",
            "available_versions": [
                1
            ]
        },
        {
            "name": "business.updated",
            "description": "Occurs when a business is updated.",
            "group": "Business",
            "available_versions": [
                1
            ]
        },
        {
            "name": "adjustment.created",
            "description": "Occurs when an adjustment is created.",
            "group": "Adjustment",
            "available_versions": [
                1
            ]
        },
        {
            "name": "adjustment.updated",
            "description": "Occurs when an adjustment is updated, the only time an adjustment will be updated is when the status changes from pending to approved or from pending to rejected.",
            "group": "Adjustment",
            "available_versions": [
                1
            ]
        },
        {
            "name": "payout.created",
            "description": "Occurs when a payout is created.",
            "group": "Payout",
            "available_versions": [
                1
            ]
        },
        {
            "name": "payout.paid",
            "description": "Occurs when a payout is paid.",
            "group": "Payout",
            "available_versions": [
                1
            ]
        },
        {
            "name": "discount.created",
            "description": "Occurs when a discount is created.",
            "group": "Discount",
            "available_versions": [
                1
            ]
        },
        {
            "name": "discount.imported",
            "description": "Occurs when a discount is imported.",
            "group": "Discount",
            "available_versions": [
                1
            ]
        },
        {
            "name": "discount.updated",
            "description": "Occurs when a discount is updated.",
            "group": "Discount",
            "available_versions": [
                1
            ]
        },
        {
            "name": "discount_group.created",
            "description": "Occurs when a discount group is created.",
            "group": "Discount Group",
            "available_versions": [
                1
            ]
        },
        {
            "name": "discount_group.updated",
            "description": "Occurs when a discount group is updated.",
            "group": "Discount Group",
            "available_versions": [
                1
            ]
        },
        {
            "name": "report.created",
            "description": "Occurs when a report is created.",
            "group": "Report",
            "available_versions": [
                1
            ]
        },
        {
            "name": "report.updated",
            "description": "Occurs when a report is updated.",
            "group": "Report",
            "available_versions": [
                1
            ]
        },
        {
            "name": "api_key.created",
            "description": "Occurs when an API key is created.",
            "group": "API Key",
            "available_versions": [
                1
            ]
        },
        {
            "name": "api_key.expired",
            "description": "Occurs when an API key has expired.",
            "group": "API Key",
            "available_versions": [
                1
            ]
        },
        {
            "name": "api_key.expiring",
            "description": "Occurs when an API key is expiring.",
            "group": "API Key",
            "available_versions": [
                1
            ]
        },
        {
            "name": "api_key.revoked",
            "description": "Occurs when an API key is revoked.",
            "group": "API Key",
            "available_versions": [
                1
            ]
        },
        {
            "name": "api_key.updated",
            "description": "Occurs when an API key is updated.",
            "group": "API Key",
            "available_versions": [
                1
            ]
        },
        {
            "name": "client_token.created",
            "description": "Occurs when a client token is created.",
            "group": "Client Token",
            "available_versions": [
                1
            ]
        },
        {
            "name": "client_token.updated",
            "description": "Occurs when a client token is updated.",
            "group": "Client Token",
            "available_versions": [
                1
            ]
        },
        {
            "name": "client_token.revoked",
            "description": "Occurs when a client token is revoked.",
            "group": "Client Token",
            "available_versions": [
                1
            ]
        }
    ],
    "endpoint_secret_key": "pdl_ntfset_01k2jjfqx34sdwsvrbj123wxx2_w4C4I+q1LScX16/ODlt39IBfOQo+20fN",
    "traffic_source": "platform"
})";
}  // namespace

UTEST(NotificationSettingsTest, Parse) {
    auto json = userver::formats::json::FromString(kNotificationSetting);
    auto setting = Parse(json, userver::formats::parse::To<NotificationSetting>());
    EXPECT_EQ(setting.id, NotificationSettingId("ntfset_01k2jjfqx34sdwsvrbj123wxx2"));
    EXPECT_EQ(setting.description, "HookDeck");
    EXPECT_EQ(setting.type, NotificationSettingType::kUrl);
    EXPECT_EQ(setting.destination, "https://hkdk.events/xieng6t7oceftj");
    EXPECT_EQ(setting.active, true);
    EXPECT_EQ(setting.api_version, 1);
    EXPECT_EQ(setting.include_sensitive_fields, true);
    EXPECT_EQ(setting.endpoint_secret_key, "pdl_ntfset_01k2jjfqx34sdwsvrbj123wxx2_w4C4I+q1LScX16/ODlt39IBfOQo+20fN");
    EXPECT_EQ(setting.traffic_source, events::TrafficSource::kPlatform);
}

}  // namespace paddle
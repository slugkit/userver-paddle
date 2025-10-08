#pragma once

#include <paddle/types/enums.hpp>

namespace paddle::permissions {

enum class Permission {
    // Address
    kAddressRead,   ///< View customer addresses and include address data in API responses.
    kAddressWrite,  ///< Create and update customer addresses.
    // Adjustment
    kAdjustmentRead,   ///< View adjustments and credit notes for transactions.
    kAdjustmentWrite,  ///< Create new adjustments for transactions.
    // Business
    kBusinessRead,   ///< View customer businesses and include business data in API responses.
    kBusinessWrite,  ///< Create and update customer businesses.
    // Customer
    kCustomerRead,   ///< View customers, including credit balances, and include customer data in API responses.
    kCustomerWrite,  ///< Create new customers and update existing customer information.
    // Customer Auth Token
    kCustomerAuthTokenWrite,  ///< Generate authentication tokens for customers.
    // Customer Portal Session
    kCustomerPortalSessionWrite,  ///< Create new customer portal sessions for customers to manage their subscriptions,
                                  ///< payment methods, and more.
    // Discount
    kDiscountRead,   ///< View discounts and include discount data in API responses.
    kDiscountWrite,  ///< Create new discounts and modify existing discount information.
    // Notification
    kNotificationRead,   ///< View event history, sent notifications, notification delivery logs, and include event data
                         ///< in API responses.
    kNotificationWrite,  ///< Replay notifications.
    // Notification Setting
    kNotificationSettingRead,   ///< View settings for notifications.
    kNotificationSettingWrite,  ///< Create, update, and delete notification settings.
    // Notification Simulation
    kNotificationSimulationRead,   ///< View notification simulations, simulation runs, and events within a simulation
                                   ///< run.
    kNotificationSimulationWrite,  ///< Create and modify notification simulations, create a run for a simulation, and
                                   ///< replay events within a run for a simulation.
    // Payment Method
    kPaymentMethodRead,   ///< View saved payment methods for customers.
    kPaymentMethodWrite,  ///< Delete payment methods for customers.
    // Price
    kPriceRead,   ///< View prices and include price data in API responses.
    kPriceWrite,  ///< Create new prices and modify existing price information.
    // Product
    kProductRead,   ///< View products and include product data in API responses.
    kProductWrite,  ///< Create new products and modify existing product information.
    // Report
    kReportRead,   ///< View and download report data.
    kReportWrite,  ///< Create new reports.
    // Subscription
    kSubscriptionRead,   ///< View subscriptions and preview subscription updates and charges.
    kSubscriptionWrite,  ///< Create one-time charges, update subscriptions, and manage subscription status.
    // Transaction
    kTransactionRead,  ///< View transactions, preview transactions and prices, access invoices, and include transaction
                       ///< data in API responses.
    kTransactionWrite,  ///< Create, update, and revise transactions, and update payment methods.
};

}  // namespace paddle::permissions

namespace paddle {

template <>
constexpr userver::utils::TrivialBiMap kEnumMap<paddle::permissions::Permission> = [](auto selector) {
    return selector()
        .Case("address.read", paddle::permissions::Permission::kAddressRead)
        .Case("address.write", paddle::permissions::Permission::kAddressWrite)
        .Case("adjustment.read", paddle::permissions::Permission::kAdjustmentRead)
        .Case("adjustment.write", paddle::permissions::Permission::kAdjustmentWrite)
        .Case("business.read", paddle::permissions::Permission::kBusinessRead)
        .Case("business.write", paddle::permissions::Permission::kBusinessWrite)
        .Case("customer.read", paddle::permissions::Permission::kCustomerRead)
        .Case("customer.write", paddle::permissions::Permission::kCustomerWrite)
        .Case("customer_auth_token.write", paddle::permissions::Permission::kCustomerAuthTokenWrite)
        .Case("customer_portal_session.write", paddle::permissions::Permission::kCustomerPortalSessionWrite)
        .Case("discount.read", paddle::permissions::Permission::kDiscountRead)
        .Case("discount.write", paddle::permissions::Permission::kDiscountWrite)
        .Case("notification.read", paddle::permissions::Permission::kNotificationRead)
        .Case("notification.write", paddle::permissions::Permission::kNotificationWrite)
        .Case("notification_setting.read", paddle::permissions::Permission::kNotificationSettingRead)
        .Case("notification_setting.write", paddle::permissions::Permission::kNotificationSettingWrite)
        .Case("notification_simulation.read", paddle::permissions::Permission::kNotificationSimulationRead)
        .Case("notification_simulation.write", paddle::permissions::Permission::kNotificationSimulationWrite)
        .Case("payment_method.read", paddle::permissions::Permission::kPaymentMethodRead)
        .Case("payment_method.write", paddle::permissions::Permission::kPaymentMethodWrite)
        .Case("price.read", paddle::permissions::Permission::kPriceRead)
        .Case("price.write", paddle::permissions::Permission::kPriceWrite)
        .Case("product.read", paddle::permissions::Permission::kProductRead)
        .Case("product.write", paddle::permissions::Permission::kProductWrite)
        .Case("report.read", paddle::permissions::Permission::kReportRead)
        .Case("report.write", paddle::permissions::Permission::kReportWrite)
        .Case("subscription.read", paddle::permissions::Permission::kSubscriptionRead)
        .Case("subscription.write", paddle::permissions::Permission::kSubscriptionWrite)
        .Case("transaction.read", paddle::permissions::Permission::kTransactionRead)
        .Case("transaction.write", paddle::permissions::Permission::kTransactionWrite);
};

}  // namespace paddle

namespace paddle::permissions {

template <typename Format>
Format Serialize(const Permission& permission, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(permission, to);
}
template <typename Value>
Permission Parse(const Value& value, userver::formats::parse::To<Permission> to) {
    return ParseEnum(value, to);
}

}  // namespace paddle::permissions

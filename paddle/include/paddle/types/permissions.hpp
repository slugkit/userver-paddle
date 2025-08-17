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

template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::permissions::Permission>
    : EnumMappingBase<paddle::permissions::Permission> {
    static constexpr DBTypeName postgres_name = "paddle.permission";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector()
            .Case("address.read", EnumType::kAddressRead)
            .Case("address.write", EnumType::kAddressWrite)
            .Case("adjustment.read", EnumType::kAdjustmentRead)
            .Case("adjustment.write", EnumType::kAdjustmentWrite)
            .Case("business.read", EnumType::kBusinessRead)
            .Case("business.write", EnumType::kBusinessWrite)
            .Case("customer.read", EnumType::kCustomerRead)
            .Case("customer.write", EnumType::kCustomerWrite)
            .Case("customer_auth_token.write", EnumType::kCustomerAuthTokenWrite)
            .Case("customer_portal_session.write", EnumType::kCustomerPortalSessionWrite)
            .Case("discount.read", EnumType::kDiscountRead)
            .Case("discount.write", EnumType::kDiscountWrite)
            .Case("notification.read", EnumType::kNotificationRead)
            .Case("notification.write", EnumType::kNotificationWrite)
            .Case("notification_setting.read", EnumType::kNotificationSettingRead)
            .Case("notification_setting.write", EnumType::kNotificationSettingWrite)
            .Case("notification_simulation.read", EnumType::kNotificationSimulationRead)
            .Case("notification_simulation.write", EnumType::kNotificationSimulationWrite)
            .Case("payment_method.read", EnumType::kPaymentMethodRead)
            .Case("payment_method.write", EnumType::kPaymentMethodWrite)
            .Case("price.read", EnumType::kPriceRead)
            .Case("price.write", EnumType::kPriceWrite)
            .Case("product.read", EnumType::kProductRead)
            .Case("product.write", EnumType::kProductWrite)
            .Case("report.read", EnumType::kReportRead)
            .Case("report.write", EnumType::kReportWrite)
            .Case("subscription.read", EnumType::kSubscriptionRead)
            .Case("subscription.write", EnumType::kSubscriptionWrite)
            .Case("transaction.read", EnumType::kTransactionRead)
            .Case("transaction.write", EnumType::kTransactionWrite);
    };
};

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

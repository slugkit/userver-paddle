#include <paddle/types/events.hpp>

namespace paddle::events {

EventCategory GetEventCategory(EventTypeName event_type) {
#pragma clang diagnostic push
#pragma clang diagnostic error "-Wswitch"
    switch (event_type) {
        // Address events
        case EventTypeName::kAddressCreated:
        case EventTypeName::kAddressImported:
        case EventTypeName::kAddressUpdated:
            return EventCategory::kAddress;
        // Adjustment events
        case EventTypeName::kAdjustmentCreated:
        case EventTypeName::kAdjustmentUpdated:
            return EventCategory::kAdjustment;
        // Business events
        case EventTypeName::kBusinessCreated:
        case EventTypeName::kBusinessImported:
        case EventTypeName::kBusinessUpdated:
            return EventCategory::kBusiness;
        // API key events
        case EventTypeName::kApiKeyCreated:
        case EventTypeName::kApiKeyExpired:
        case EventTypeName::kApiKeyExpiring:
        case EventTypeName::kApiKeyRevoked:
        case EventTypeName::kApiKeyUpdated:
            return EventCategory::kApiKey;
        // Client token events
        case EventTypeName::kClientTokenCreated:
        case EventTypeName::kClientTokenRevoked:
        case EventTypeName::kClientTokenUpdated:
            return EventCategory::kClientToken;
        // Customer events
        case EventTypeName::kCustomerCreated:
        case EventTypeName::kCustomerImported:
        case EventTypeName::kCustomerUpdated:
            return EventCategory::kCustomer;
        // Discount events
        case EventTypeName::kDiscountCreated:
        case EventTypeName::kDiscountImported:
        case EventTypeName::kDiscountUpdated:
            return EventCategory::kDiscount;
        // Discount group events
        case EventTypeName::kDiscountGroupCreated:
        case EventTypeName::kDiscountGroupUpdated:
            return EventCategory::kDiscountGroup;
        // Payment method events
        case EventTypeName::kPaymentMethodSaved:
        case EventTypeName::kPaymentMethodDeleted:
            return EventCategory::kPaymentMethod;
        // Payout events
        case EventTypeName::kPayoutCreated:
        case EventTypeName::kPayoutPaid:
            return EventCategory::kPayout;
        // Price events
        case EventTypeName::kPriceCreated:
        case EventTypeName::kPriceImported:
        case EventTypeName::kPriceUpdated:
            return EventCategory::kPrice;
        // Product events
        case EventTypeName::kProductCreated:
        case EventTypeName::kProductImported:
        case EventTypeName::kProductUpdated:
            return EventCategory::kProduct;
        // Report events
        case EventTypeName::kReportCreated:
        case EventTypeName::kReportUpdated:
            return EventCategory::kReport;
            // Subscription events
        case EventTypeName::kSubscriptionActivated:
        case EventTypeName::kSubscriptionCanceled:
        case EventTypeName::kSubscriptionCreated:
        case EventTypeName::kSubscriptionImported:
        case EventTypeName::kSubscriptionPastDue:
        case EventTypeName::kSubscriptionPaused:
        case EventTypeName::kSubscriptionResumed:
        case EventTypeName::kSubscriptionUpdated:
        case EventTypeName::kSubscriptionTrialing:
            return EventCategory::kSubscription;
        // Transaction events
        case EventTypeName::kTransactionBilled:
        case EventTypeName::kTransactionCanceled:
        case EventTypeName::kTransactionCompleted:
        case EventTypeName::kTransactionCreated:
        case EventTypeName::kTransactionPaid:
        case EventTypeName::kTransactionPastDue:
        case EventTypeName::kTransactionPaymentFailed:
        case EventTypeName::kTransactionReady:
        case EventTypeName::kTransactionRevised:
        case EventTypeName::kTransactionUpdated:
            return EventCategory::kTransaction;
    }
#pragma clang diagnostic pop
    return EventCategory::kUnknown;
}

}  // namespace paddle::events
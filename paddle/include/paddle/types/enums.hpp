#pragma once

#include <paddle/types/formats.hpp>

#include <userver/logging/log_helper.hpp>
#include <userver/storages/postgres/io/enum_types.hpp>
#include <userver/utils/trivial_map.hpp>

namespace paddle {

enum class CatalogType {
    kStandard,
    kCustom,
};

enum class Status {
    kActive,
    kArchived,
};

enum class Interval {
    kDay,
    kWeek,
    kMonth,
    kYear,
};

enum class TransactionStatus {
    kDraft,
    kReady,
    kBilled,
    kPaid,
    kCompleted,
    kCanceled,
    kPastDue,
};

enum class TransactionOrigin {
    kApi,                 ///< Transaction created via the Paddle API.
    kSubscriptionCharge,  ///< Transaction created automatically by Paddle as a result of a one-time charge for a
                          ///< subscription.
    kSubscriptionPaymentMethodChange,  ///< Transaction created automatically as part of updating a payment method. May
                                       ///< be a zero value transaction.
    kSubscriptionRecurring,  ///< Transaction created automatically by Paddle as a result of a subscription renewal.
    kSubscriptionUpdate,  ///< Transaction created automatically by Paddle as a result of an update to a subscription.
    kWeb,                 ///< Transaction created automatically by Paddle.js for a checkout.
};

enum class PaymentAttemptStatus {
    kAuthorized,         ///< Authorized but not captured. Payment attempt is incomplete.
    kAuthorizedFlagged,  ///< Authorized but not captured because it has been flagged as potentially fraudulent. Payment
                         ///< attempt is incomplete.
    kCanceled,  ///< Previously authorized payment attempt has been canceled. Typically when `authorized_flagged`
                ///< payment attempts are rejected.
    kCaptured,  ///< Payment captured successfully. Payment attempt is complete.
    kError,     ///< Something went wrong and the payment attempt was unsuccessful. Check the `error_code` for more
                ///< information.
    kActionRequired,  ///< Customer must complete an action for this payment attempt to proceed. Typically means that
                      ///< the payment attempt requires 3DS.
    kPendingNoActionRequired,  ///< Response required from the bank or payment provider. Transaction is pending.
    kCreated,                  ///< New payment attempt created.
    kUnknown,                  ///< Payment attempt status not known.
    kDropped,                  ///< Payment attempt dropped by Paddle.
};

/// Reason why a payment attempt failed.
enum class PaymentAttemptErrorCode {
    kAlreadyCanceled,  ///< Cancellation not possible because the amount has already been canceled. Not typically
                       ///< returned for payments.
    kAlreadyRefunded,  ///< Refund is not possible because the amount has already been refunded. Not typically returned
                       ///< for payments.
    kAuthenticationFailed,  ///< Payment required a 3DS2 authentication challenge. The customer completed the challenge
                            ///< but was not successful.
    kBlockedCard,  ///< Payment method issuer has indicated that the card cannot be used as it is frozen, lost, damaged,
                   ///< or stolen.
    kCanceled,     ///< Customer has requested that the mandate for recurring payments be canceled.
    kDeclined,     ///< Payment method has been declined, with no other information returned.
    kDeclinedNotRetryable,  ///< Payment method has been declined, and the issuer has indicated that it should not be
                            ///< retried. This could mean the account is closed or the customer revoked authorization to
                            ///< charge the payment method.
    kExpiredCard,  ///< Payment method issuer has indicated that this card is expired. Expired cards may also return
                   ///< `invalid_payment_details`, depending on how a payment is routed.
    kFraud,  ///< Payment method issuer or payment service provider flagged this payment as potentially fraudulent.
    kInvalidAmount,  ///< Payment method issuer or payment service provider cannot process a payment that is this high
                     ///< or low.
    kInvalidPaymentDetails,  ///< Payment service provider has indicated the payment method isn't valid. This typically
                             ///< means that it's expired. `expired_card` is returned by the payment method issuer,
                             ///< rather than the payment service provider.
    kIssuerUnavailable,      ///< Payment service provider couldn't reach the payment method issuer.
    kNotEnoughBalance,       ///< Payment method declined because of insufficient funds, or fund limits being reached.
    kPreferredNetworkNotSupported,  ///< Payment method has been declined because the network scheme that the customer
                                    ///< selected isn't supported by the payment service provider.
    kPspError,  ///< Something went wrong with the payment service provider, with no other information returned.
    kRedactedPaymentMethod,  ///< Payment service provider didn't receive payment method information as they've been
                             ///< redacted.
    kSystemError,  ///< Something went wrong with the Paddle platform. Try again later, or check status.paddle.com.
    kTransactionNotPermitted,  ///< Payment method issuer doesn't allow this kind of payment because of limits on the
                               ///< account, or legal or compliance reasons.
    kUnknown,                  ///< Payment attempt unsuccessful, with no other information returned.
};

/// Type of payment method used for this payment attempt.
enum class PaymentMethodType {
    kAlipay,        ///< Alipay, popular in China.
    kApplePay,      ///< Apple Pay on a supported Apple device.
    kBancontact,    ///< Bancontact, popular in Belgium.
    kCard,          ///< Credit or debit card.
    kGooglePay,     ///< Google Pay on a supported Android device, Chromebook, or Google Chrome browser.
    kIdeal,         ///< iDEAL, popular in the Netherlands.
    kKoreaLocal,    ///< Korean payment methods, which includes over 20 payment options for the Korean market. Check
                    ///< `underlying_payment_method.korea_local` for information about the Korean payment method used to
                    ///< pay.
    kOffline,       ///< Payment recorded offline.
    kPaypal,        ///< PayPal.
    kUnknown,       ///< Payment method not known.
    kWireTransfer,  ///< Wire transfer, sometimes called bank transfer.
};

enum class SubscriptionStatus {
    kActive,
    kCancelled,
    kPastDue,
    kPaused,
    kTrialing,
};

enum class SubscriptionItemStatus {
    kActive,
    kInactive,
    kTrialing,
};

enum class CollectionMode {
    kManual,
    kAutomatic,
};

enum class ScheduledChangeAction {
    kCancel,
    kPause,
    kResume,
};

enum class NotificationSettingType {
    kUrl,
    kEmail,
};

enum class ClientTokenStatus {
    kActive,
    kRevoked,
};

}  // namespace paddle

// CatalogType
template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::CatalogType> : EnumMappingBase<paddle::CatalogType> {
    static constexpr DBTypeName postgres_name = "paddle.catalog_type";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector().Case("standard", EnumType::kStandard).Case("custom", EnumType::kCustom);
    };
};

// Status
template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::Status> : EnumMappingBase<paddle::Status> {
    static constexpr DBTypeName postgres_name = "paddle.status";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector().Case("active", EnumType::kActive).Case("archived", EnumType::kArchived);
    };
};

// Interval
template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::Interval> : EnumMappingBase<paddle::Interval> {
    static constexpr DBTypeName postgres_name = "paddle.interval";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector()
            .Case("day", EnumType::kDay)
            .Case("week", EnumType::kWeek)
            .Case("month", EnumType::kMonth)
            .Case("year", EnumType::kYear);
    };
};

// TransactionStatus
template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::TransactionStatus>
    : EnumMappingBase<paddle::TransactionStatus> {
    static constexpr DBTypeName postgres_name = "paddle.transaction_status";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector()
            .Case("draft", EnumType::kDraft)
            .Case("ready", EnumType::kReady)
            .Case("billed", EnumType::kBilled)
            .Case("paid", EnumType::kPaid)
            .Case("completed", EnumType::kCompleted)
            .Case("canceled", EnumType::kCanceled)
            .Case("past_due", EnumType::kPastDue);
    };
};

// TransactionOrigin
template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::TransactionOrigin>
    : EnumMappingBase<paddle::TransactionOrigin> {
    static constexpr DBTypeName postgres_name = "paddle.transaction_origin";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector()
            .Case("api", EnumType::kApi)
            .Case("subscription_charge", EnumType::kSubscriptionCharge)
            .Case("subscription_payment_method_change", EnumType::kSubscriptionPaymentMethodChange)
            .Case("subscription_recurring", EnumType::kSubscriptionRecurring)
            .Case("subscription_update", EnumType::kSubscriptionUpdate)
            .Case("web", EnumType::kWeb);
    };
};

// PaymentAttemptStatus
template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::PaymentAttemptStatus>
    : EnumMappingBase<paddle::PaymentAttemptStatus> {
    static constexpr DBTypeName postgres_name = "paddle.payment_attempt_status";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector()
            .Case("authorized", EnumType::kAuthorized)
            .Case("authorized_flagged", EnumType::kAuthorizedFlagged)
            .Case("canceled", EnumType::kCanceled)
            .Case("captured", EnumType::kCaptured)
            .Case("error", EnumType::kError)
            .Case("action_required", EnumType::kActionRequired)
            .Case("pending_no_action_required", EnumType::kPendingNoActionRequired)
            .Case("created", EnumType::kCreated)
            .Case("unknown", EnumType::kUnknown)
            .Case("dropped", EnumType::kDropped);
    };
};

// PaymentAttemptErrorCode
template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::PaymentAttemptErrorCode>
    : EnumMappingBase<paddle::PaymentAttemptErrorCode> {
    static constexpr DBTypeName postgres_name = "paddle.payment_attempt_error_code";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector()
            .Case("already_canceled", EnumType::kAlreadyCanceled)
            .Case("already_refunded", EnumType::kAlreadyRefunded)
            .Case("authentication_failed", EnumType::kAuthenticationFailed)
            .Case("blocked_card", EnumType::kBlockedCard)
            .Case("canceled", EnumType::kCanceled)
            .Case("declined", EnumType::kDeclined)
            .Case("declined_not_retryable", EnumType::kDeclinedNotRetryable)
            .Case("expired_card", EnumType::kExpiredCard)
            .Case("fraud", EnumType::kFraud)
            .Case("invalid_amount", EnumType::kInvalidAmount)
            .Case("invalid_payment_details", EnumType::kInvalidPaymentDetails)
            .Case("issuer_unavailable", EnumType::kIssuerUnavailable)
            .Case("not_enough_balance", EnumType::kNotEnoughBalance)
            .Case("preferred_network_not_supported", EnumType::kPreferredNetworkNotSupported)
            .Case("psp_error", EnumType::kPspError)
            .Case("redacted_payment_method", EnumType::kRedactedPaymentMethod)
            .Case("system_error", EnumType::kSystemError)
            .Case("transaction_not_permitted", EnumType::kTransactionNotPermitted)
            .Case("unknown", EnumType::kUnknown);
    };
};

// PaymentMethodType
template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::PaymentMethodType>
    : EnumMappingBase<paddle::PaymentMethodType> {
    static constexpr DBTypeName postgres_name = "paddle.payment_method_type";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector()
            .Case("alipay", EnumType::kAlipay)
            .Case("apple_pay", EnumType::kApplePay)
            .Case("bancontact", EnumType::kBancontact)
            .Case("card", EnumType::kCard)
            .Case("google_pay", EnumType::kGooglePay)
            .Case("ideal", EnumType::kIdeal)
            .Case("korea_local", EnumType::kKoreaLocal)
            .Case("offline", EnumType::kOffline)
            .Case("paypal", EnumType::kPaypal)
            .Case("unknown", EnumType::kUnknown)
            .Case("wire_transfer", EnumType::kWireTransfer);
    };
};

// SubscriptionStatus
template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::SubscriptionStatus>
    : EnumMappingBase<paddle::SubscriptionStatus> {
    static constexpr DBTypeName postgres_name = "paddle.subscription_status";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector()
            .Case("active", EnumType::kActive)
            .Case("cancelled", EnumType::kCancelled)
            .Case("past_due", EnumType::kPastDue)
            .Case("paused", EnumType::kPaused)
            .Case("trialing", EnumType::kTrialing);
    };
};

// SubscriptionItemStatus
template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::SubscriptionItemStatus>
    : EnumMappingBase<paddle::SubscriptionItemStatus> {
    static constexpr DBTypeName postgres_name = "paddle.subscription_item_status";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector()
            .Case("active", EnumType::kActive)
            .Case("inactive", EnumType::kInactive)
            .Case("trialing", EnumType::kTrialing);
    };
};

// CollectionMode
template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::CollectionMode> : EnumMappingBase<paddle::CollectionMode> {
    static constexpr DBTypeName postgres_name = "paddle.collection_mode";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector().Case("manual", EnumType::kManual).Case("automatic", EnumType::kAutomatic);
    };
};

// ScheduledChangeAction
template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::ScheduledChangeAction>
    : EnumMappingBase<paddle::ScheduledChangeAction> {
    static constexpr DBTypeName postgres_name = "paddle.scheduled_change_action";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector()
            .Case("cancel", EnumType::kCancel)
            .Case("pause", EnumType::kPause)
            .Case("resume", EnumType::kResume);
    };
};

// NotificationSettingType
template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::NotificationSettingType>
    : EnumMappingBase<paddle::NotificationSettingType> {
    static constexpr DBTypeName postgres_name = "paddle.notification_setting_type";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector().Case("url", EnumType::kUrl).Case("email", EnumType::kEmail);
    };
};

// ClientTokenStatus
template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::ClientTokenStatus>
    : EnumMappingBase<paddle::ClientTokenStatus> {
    static constexpr DBTypeName postgres_name = "paddle.client_token_status";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector().Case("active", EnumType::kActive).Case("revoked", EnumType::kRevoked);
    };
};

namespace paddle {

template <typename Enum, typename Format>
Format SerializeEnum(const Enum& enum_value, userver::formats::serialize::To<Format>) {
    using EnumMap = userver::storages::postgres::io::detail::EnumerationMap<Enum>;
    typename Format::Builder builder(EnumMap::GetLiteral(enum_value));
    return builder.ExtractValue();
}

template <typename Enum, typename Value>
Enum ParseEnum(const Value& value, userver::formats::parse::To<Enum>) {
    using EnumMap = userver::storages::postgres::io::detail::EnumerationMap<Enum>;
    auto enum_value = value.template As<std::string>();
    return EnumMap::GetEnumerator(enum_value);
}

template <typename Enum>
userver::logging::LogHelper& LogEnum(userver::logging::LogHelper& helper, const Enum& enum_value) {
    using EnumMap = userver::storages::postgres::io::detail::EnumerationMap<Enum>;
    helper << EnumMap::GetLiteral(enum_value);
    return helper;
}

template <typename Enum>
auto EnumToString(const Enum& enum_value) {
    using EnumMap = userver::storages::postgres::io::detail::EnumerationMap<Enum>;
    return EnumMap::GetLiteral(enum_value);
}

// CatalogType
template <typename Format>
Format Serialize(const CatalogType& type, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(type, to);
}

template <typename Value>
CatalogType Parse(const Value& value, userver::formats::parse::To<CatalogType> to) {
    return ParseEnum(value, to);
}

// Status
template <typename Format>
Format Serialize(const Status& status, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(status, to);
}

template <typename Value>
Status Parse(const Value& value, userver::formats::parse::To<Status> to) {
    return ParseEnum(value, to);
}

// Interval
template <typename Format>
Format Serialize(const Interval& interval, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(interval, to);
}

template <typename Value>
Interval Parse(const Value& value, userver::formats::parse::To<Interval> to) {
    return ParseEnum(value, to);
}

// TransactionStatus
template <typename Format>
Format Serialize(const TransactionStatus& status, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(status, to);
}

template <typename Value>
TransactionStatus Parse(const Value& value, userver::formats::parse::To<TransactionStatus> to) {
    return ParseEnum(value, to);
}

// TransactionOrigin
template <typename Format>
Format Serialize(const TransactionOrigin& origin, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(origin, to);
}

template <typename Value>
TransactionOrigin Parse(const Value& value, userver::formats::parse::To<TransactionOrigin> to) {
    return ParseEnum(value, to);
}

// PaymentAttemptStatus
template <typename Format>
Format Serialize(const PaymentAttemptStatus& status, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(status, to);
}

template <typename Value>
PaymentAttemptStatus Parse(const Value& value, userver::formats::parse::To<PaymentAttemptStatus> to) {
    return ParseEnum(value, to);
}

// PaymentAttemptErrorCode
template <typename Format>
Format Serialize(const PaymentAttemptErrorCode& error_code, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(error_code, to);
}

template <typename Value>
PaymentAttemptErrorCode Parse(const Value& value, userver::formats::parse::To<PaymentAttemptErrorCode> to) {
    return ParseEnum(value, to);
}

// PaymentMethodType
template <typename Format>
Format Serialize(const PaymentMethodType& type, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(type, to);
}

template <typename Value>
PaymentMethodType Parse(const Value& value, userver::formats::parse::To<PaymentMethodType> to) {
    return ParseEnum(value, to);
}

// SubscriptionStatus
template <typename Format>
Format Serialize(const SubscriptionStatus& status, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(status, to);
}

template <typename Value>
SubscriptionStatus Parse(const Value& value, userver::formats::parse::To<SubscriptionStatus> to) {
    return ParseEnum(value, to);
}

// SubscriptionItemStatus
template <typename Format>
Format Serialize(const SubscriptionItemStatus& status, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(status, to);
}

template <typename Value>
SubscriptionItemStatus Parse(const Value& value, userver::formats::parse::To<SubscriptionItemStatus> to) {
    return ParseEnum(value, to);
}

// CollectionMode
template <typename Format>
Format Serialize(const CollectionMode& mode, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(mode, to);
}

template <typename Value>
CollectionMode Parse(const Value& value, userver::formats::parse::To<CollectionMode> to) {
    return ParseEnum(value, to);
}

// ScheduledChangeAction
template <typename Format>
Format Serialize(const ScheduledChangeAction& action, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(action, to);
}

template <typename Value>
ScheduledChangeAction Parse(const Value& value, userver::formats::parse::To<ScheduledChangeAction> to) {
    return ParseEnum(value, to);
}

// NotificationSettingType
template <typename Format>
Format Serialize(const NotificationSettingType& type, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(type, to);
}

template <typename Value>
NotificationSettingType Parse(const Value& value, userver::formats::parse::To<NotificationSettingType> to) {
    return ParseEnum(value, to);
}

// ClientTokenStatus

template <typename Format>
Format Serialize(const ClientTokenStatus& status, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(status, to);
}

template <typename Value>
ClientTokenStatus Parse(const Value& value, userver::formats::parse::To<ClientTokenStatus> to) {
    return ParseEnum(value, to);
}

}  // namespace paddle
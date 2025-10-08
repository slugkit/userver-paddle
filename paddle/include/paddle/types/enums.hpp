#pragma once

#include <paddle/types/formats.hpp>

#include <userver/logging/log_helper.hpp>
#include <userver/utils/trivial_map.hpp>

namespace paddle {

template <typename Enum>
requires std::is_enum_v<Enum>
constexpr userver::utils::TrivialBiMap kEnumMap = [](auto) { static_assert(false, "EnumMap not implemented"); };

template <typename Enum>
auto EnumToString(const Enum& enum_value) -> std::string {
    using EnumUnderlyingType = std::underlying_type_t<Enum>;
    auto literal_ptr = kEnumMap<Enum>.TryFind(enum_value);
    if (literal_ptr.has_value()) {
        return std::string(*literal_ptr);
    }
    throw std::invalid_argument(fmt::format("Invalid enum value: {}", static_cast<EnumUnderlyingType>(enum_value)));
}

template <typename Enum, typename Format>
auto SerializeEnum(const Enum& enum_value, userver::formats::serialize::To<Format>) -> Format {
    typename Format::Builder builder(EnumToString(enum_value));
    return builder.ExtractValue();
}

template <typename Enum, typename Value>
auto ParseEnum(const Value& value, userver::formats::parse::To<Enum>) -> Enum {
    auto enum_value = value.template As<std::string>();
    auto enum_value_ptr = kEnumMap<Enum>.TryFind(enum_value);
    if (enum_value_ptr.has_value()) {
        return *enum_value_ptr;
    }
    throw std::invalid_argument(fmt::format("Invalid enum value: {}", enum_value));
}

template <typename Enum>
auto LogEnum(userver::logging::LogHelper& helper, const Enum& enum_value) -> userver::logging::LogHelper& {
    return helper << EnumToString(enum_value);
}

enum class CatalogType {
    kStandard,
    kCustom,
};

template <>
constexpr userver::utils::TrivialBiMap kEnumMap<CatalogType> = [](auto selector) {
    return selector().Case("standard", CatalogType::kStandard).Case("custom", CatalogType::kCustom);
};

enum class Status {
    kActive,
    kArchived,
};

template <>
constexpr userver::utils::TrivialBiMap kEnumMap<Status> =
    [](auto selector) { return selector().Case("active", Status::kActive).Case("archived", Status::kArchived); };

enum class Interval {
    kDay,
    kWeek,
    kMonth,
    kYear,
};

template <>
constexpr userver::utils::TrivialBiMap kEnumMap<Interval> = [](auto selector) {
    return selector()
        .Case("day", Interval::kDay)
        .Case("week", Interval::kWeek)
        .Case("month", Interval::kMonth)
        .Case("year", Interval::kYear);
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

template <>
constexpr userver::utils::TrivialBiMap kEnumMap<TransactionStatus> = [](auto selector) {
    return selector()
        .Case("draft", TransactionStatus::kDraft)
        .Case("ready", TransactionStatus::kReady)
        .Case("billed", TransactionStatus::kBilled)
        .Case("paid", TransactionStatus::kPaid)
        .Case("completed", TransactionStatus::kCompleted)
        .Case("canceled", TransactionStatus::kCanceled)
        .Case("past_due", TransactionStatus::kPastDue);
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

template <>
constexpr userver::utils::TrivialBiMap kEnumMap<TransactionOrigin> = [](auto selector) {
    return selector()
        .Case("api", TransactionOrigin::kApi)
        .Case("subscription_charge", TransactionOrigin::kSubscriptionCharge)
        .Case("subscription_payment_method_change", TransactionOrigin::kSubscriptionPaymentMethodChange)
        .Case("subscription_recurring", TransactionOrigin::kSubscriptionRecurring)
        .Case("subscription_update", TransactionOrigin::kSubscriptionUpdate)
        .Case("web", TransactionOrigin::kWeb);
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

template <>
constexpr userver::utils::TrivialBiMap kEnumMap<PaymentAttemptStatus> = [](auto selector) {
    return selector()
        .Case("authorized", PaymentAttemptStatus::kAuthorized)
        .Case("authorized_flagged", PaymentAttemptStatus::kAuthorizedFlagged)
        .Case("canceled", PaymentAttemptStatus::kCanceled)
        .Case("captured", PaymentAttemptStatus::kCaptured)
        .Case("error", PaymentAttemptStatus::kError)
        .Case("action_required", PaymentAttemptStatus::kActionRequired)
        .Case("pending_no_action_required", PaymentAttemptStatus::kPendingNoActionRequired)
        .Case("created", PaymentAttemptStatus::kCreated)
        .Case("unknown", PaymentAttemptStatus::kUnknown)
        .Case("dropped", PaymentAttemptStatus::kDropped);
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

template <>
constexpr userver::utils::TrivialBiMap kEnumMap<PaymentAttemptErrorCode> = [](auto selector) {
    return selector()
        .Case("already_canceled", PaymentAttemptErrorCode::kAlreadyCanceled)
        .Case("already_refunded", PaymentAttemptErrorCode::kAlreadyRefunded)
        .Case("authentication_failed", PaymentAttemptErrorCode::kAuthenticationFailed)
        .Case("blocked_card", PaymentAttemptErrorCode::kBlockedCard)
        .Case("canceled", PaymentAttemptErrorCode::kCanceled)
        .Case("declined", PaymentAttemptErrorCode::kDeclined)
        .Case("declined_not_retryable", PaymentAttemptErrorCode::kDeclinedNotRetryable)
        .Case("expired_card", PaymentAttemptErrorCode::kExpiredCard)
        .Case("fraud", PaymentAttemptErrorCode::kFraud)
        .Case("invalid_amount", PaymentAttemptErrorCode::kInvalidAmount)
        .Case("invalid_payment_details", PaymentAttemptErrorCode::kInvalidPaymentDetails)
        .Case("issuer_unavailable", PaymentAttemptErrorCode::kIssuerUnavailable)
        .Case("not_enough_balance", PaymentAttemptErrorCode::kNotEnoughBalance)
        .Case("preferred_network_not_supported", PaymentAttemptErrorCode::kPreferredNetworkNotSupported)
        .Case("psp_error", PaymentAttemptErrorCode::kPspError)
        .Case("redacted_payment_method", PaymentAttemptErrorCode::kRedactedPaymentMethod)
        .Case("system_error", PaymentAttemptErrorCode::kSystemError)
        .Case("transaction_not_permitted", PaymentAttemptErrorCode::kTransactionNotPermitted)
        .Case("unknown", PaymentAttemptErrorCode::kUnknown);
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

template <>
constexpr userver::utils::TrivialBiMap kEnumMap<PaymentMethodType> = [](auto selector) {
    return selector()
        .Case("alipay", PaymentMethodType::kAlipay)
        .Case("apple_pay", PaymentMethodType::kApplePay)
        .Case("bancontact", PaymentMethodType::kBancontact)
        .Case("card", PaymentMethodType::kCard)
        .Case("google_pay", PaymentMethodType::kGooglePay)
        .Case("ideal", PaymentMethodType::kIdeal)
        .Case("korea_local", PaymentMethodType::kKoreaLocal)
        .Case("offline", PaymentMethodType::kOffline)
        .Case("paypal", PaymentMethodType::kPaypal)
        .Case("unknown", PaymentMethodType::kUnknown)
        .Case("wire_transfer", PaymentMethodType::kWireTransfer);
};

enum class SubscriptionStatus {
    kActive,
    kCancelled,
    kPastDue,
    kPaused,
    kTrialing,
};

template <>
constexpr userver::utils::TrivialBiMap kEnumMap<SubscriptionStatus> = [](auto selector) {
    return selector()
        .Case("active", SubscriptionStatus::kActive)
        .Case("cancelled", SubscriptionStatus::kCancelled)
        .Case("past_due", SubscriptionStatus::kPastDue)
        .Case("paused", SubscriptionStatus::kPaused)
        .Case("trialing", SubscriptionStatus::kTrialing);
};

enum class SubscriptionItemStatus {
    kActive,
    kInactive,
    kTrialing,
};

template <>
constexpr userver::utils::TrivialBiMap kEnumMap<SubscriptionItemStatus> = [](auto selector) {
    return selector()
        .Case("active", SubscriptionItemStatus::kActive)
        .Case("inactive", SubscriptionItemStatus::kInactive)
        .Case("trialing", SubscriptionItemStatus::kTrialing);
};

enum class CollectionMode {
    kManual,
    kAutomatic,
};

template <>
constexpr userver::utils::TrivialBiMap kEnumMap<CollectionMode> = [](auto selector) {
    return selector().Case("manual", CollectionMode::kManual).Case("automatic", CollectionMode::kAutomatic);
};

enum class ScheduledChangeAction {
    kCancel,
    kPause,
    kResume,
};

template <>
constexpr userver::utils::TrivialBiMap kEnumMap<ScheduledChangeAction> = [](auto selector) {
    return selector()
        .Case("cancel", ScheduledChangeAction::kCancel)
        .Case("pause", ScheduledChangeAction::kPause)
        .Case("resume", ScheduledChangeAction::kResume);
};

enum class NotificationSettingType {
    kUrl,
    kEmail,
};

template <>
constexpr userver::utils::TrivialBiMap kEnumMap<NotificationSettingType> = [](auto selector) {
    return selector().Case("url", NotificationSettingType::kUrl).Case("email", NotificationSettingType::kEmail);
};

enum class ClientTokenStatus {
    kActive,
    kRevoked,
};

template <>
constexpr userver::utils::TrivialBiMap kEnumMap<ClientTokenStatus> = [](auto selector) {
    return selector().Case("active", ClientTokenStatus::kActive).Case("revoked", ClientTokenStatus::kRevoked);
};

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
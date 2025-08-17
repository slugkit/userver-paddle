#pragma once

#include <paddle/types/enums.hpp>
#include <paddle/types/ids.hpp>
#include <paddle/types/response.hpp>
#include <paddle/types/timestamp.hpp>

namespace paddle::money {

/// Describes how this payment method was saved.
enum class PaymentMethodOrigin {
    kSavedDuringPurchase,  ///< The customer chose to save this payment method while purchasing a one-time item.
    kSubscription,  ///< The customer purchased a subscription, so this payment method was saved for future purchases.
};

enum class PaymentMethodDeletionReason {
    kReplacedByNewerVersion,  ///< Customer supplied payment details that matched this saved payment method entity, so
                              ///< Paddle created a replacement entity and deleted this one.
    kApi,                     ///< Saved payment method deleted using the API.
};

struct Card {
    std::string type;
    std::string last4;
    std::int32_t expiry_month;
    std::int32_t expiry_year;
    std::string cardholder_name;
};

struct PaymentMethodEventPayload {
    PaymentMethodId id;
    CustomerId customer_id;
    AddressId address_id;
    PaymentMethodType type;
    PaymentMethodOrigin origin;
    Timestamp saved_at;
    Timestamp updated_at;
    std::optional<PaymentMethodDeletionReason> deletion_reason;
};

struct SavedPaymentMethod {
    PaymentMethodId id;
    CustomerId customer_id;
    AddressId address_id;
    PaymentMethodType type;
    PaymentMethodOrigin origin;
    std::optional<Card> card;
    JSON paypal;
    JSON underlying_details;
    Timestamp saved_at;
    Timestamp updated_at;
};

}  // namespace paddle::money

// PaymentMethodOrigin
template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::money::PaymentMethodOrigin>
    : EnumMappingBase<paddle::money::PaymentMethodOrigin> {
    static constexpr DBTypeName postgres_name = "paddle.payment_method_origin";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector()
            .Case("saved_during_purchase", EnumType::kSavedDuringPurchase)
            .Case("subscription", EnumType::kSubscription);
    };
};

// PaymentMethodDeletionReason
template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::money::PaymentMethodDeletionReason>
    : EnumMappingBase<paddle::money::PaymentMethodDeletionReason> {
    static constexpr DBTypeName postgres_name = "paddle.payment_method_deletion_reason";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector()
            .Case("replaced_by_newer_version", EnumType::kReplacedByNewerVersion)
            .Case("api", EnumType::kApi);
    };
};

namespace paddle::money {

// PaymentMethodOrigin
template <typename Format>
Format Serialize(const PaymentMethodOrigin& origin, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(origin, to);
}

template <typename Value>
PaymentMethodOrigin Parse(const Value& value, userver::formats::parse::To<PaymentMethodOrigin> to) {
    return ParseEnum(value, to);
}

// PaymentMethodDeletionReason
template <typename Format>
Format Serialize(const PaymentMethodDeletionReason& deletion_reason, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(deletion_reason, to);
}

template <typename Value>
PaymentMethodDeletionReason Parse(const Value& value, userver::formats::parse::To<PaymentMethodDeletionReason> to) {
    return ParseEnum(value, to);
}

// Card
template <typename Format>
Format Serialize(const Card& card, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["type"] = card.type;
    builder["last4"] = card.last4;
    builder["expiry_month"] = card.expiry_month;
    builder["expiry_year"] = card.expiry_year;
    builder["cardholder_name"] = card.cardholder_name;
    return builder.ExtractValue();
}

template <typename Value>
Card Parse(const Value& value, userver::formats::parse::To<Card>) {
    Card card;
    card.type = value["type"].template As<std::string>();
    card.last4 = value["last4"].template As<std::string>();
    card.expiry_month = value["expiry_month"].template As<std::int32_t>();
    card.expiry_year = value["expiry_year"].template As<std::int32_t>();
    card.cardholder_name = value["cardholder_name"].template As<std::string>();
    return card;
}

// PaymentMethodEvent
template <typename Format>
Format
Serialize(const PaymentMethodEventPayload& payment_method_event_payload, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["id"] = payment_method_event_payload.id;
    builder["customer_id"] = payment_method_event_payload.customer_id;
    builder["address_id"] = payment_method_event_payload.address_id;
    builder["type"] = payment_method_event_payload.type;
    builder["origin"] = payment_method_event_payload.origin;
    builder["saved_at"] = payment_method_event_payload.saved_at;
    builder["updated_at"] = payment_method_event_payload.updated_at;
    if (payment_method_event_payload.deletion_reason) {
        builder["deletion_reason"] = *payment_method_event_payload.deletion_reason;
    }
    return builder.ExtractValue();
}

template <typename Value>
PaymentMethodEventPayload Parse(const Value& value, userver::formats::parse::To<PaymentMethodEventPayload>) {
    PaymentMethodEventPayload payment_method_event_payload;
    payment_method_event_payload.id = value["id"].template As<PaymentMethodId>();
    payment_method_event_payload.customer_id = value["customer_id"].template As<CustomerId>();
    payment_method_event_payload.address_id = value["address_id"].template As<AddressId>();
    payment_method_event_payload.type = value["type"].template As<PaymentMethodType>();
    payment_method_event_payload.origin = value["origin"].template As<PaymentMethodOrigin>();
    payment_method_event_payload.saved_at = value["saved_at"].template As<Timestamp>();
    payment_method_event_payload.updated_at = value["updated_at"].template As<Timestamp>();
    if (value.HasMember("deletion_reason")) {
        payment_method_event_payload.deletion_reason =
            value["deletion_reason"].template As<PaymentMethodDeletionReason>();
    }
    return payment_method_event_payload;
}

// SavedPaymentMethod
template <typename Format>
Format Serialize(const SavedPaymentMethod& saved_payment_method, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["id"] = saved_payment_method.id;
    builder["customer_id"] = saved_payment_method.customer_id;
    builder["address_id"] = saved_payment_method.address_id;
    builder["type"] = saved_payment_method.type;
    builder["origin"] = saved_payment_method.origin;
    builder["saved_at"] = saved_payment_method.saved_at;
    builder["updated_at"] = saved_payment_method.updated_at;
    builder["card"] = saved_payment_method.card;
    builder["paypal"] = saved_payment_method.paypal;
    builder["underlying_details"] = saved_payment_method.underlying_details;
    return builder.ExtractValue();
}

template <typename Value>
SavedPaymentMethod Parse(const Value& value, userver::formats::parse::To<SavedPaymentMethod>) {
    SavedPaymentMethod saved_payment_method;
    saved_payment_method.id = value["id"].template As<PaymentMethodId>();
    saved_payment_method.customer_id = value["customer_id"].template As<CustomerId>();
    saved_payment_method.address_id = value["address_id"].template As<AddressId>();
    saved_payment_method.type = value["type"].template As<PaymentMethodType>();
    saved_payment_method.origin = value["origin"].template As<PaymentMethodOrigin>();
    if (value.HasMember("card")) {
        saved_payment_method.card = value["card"].template As<std::optional<Card>>();
    }
    if (value.HasMember("paypal")) {
        saved_payment_method.paypal = value["paypal"].template As<JSON>();
    }
    if (value.HasMember("underlying_details")) {
        saved_payment_method.underlying_details = value["underlying_details"].template As<JSON>();
    }
    saved_payment_method.saved_at = value["saved_at"].template As<Timestamp>();
    saved_payment_method.updated_at = value["updated_at"].template As<Timestamp>();
    return saved_payment_method;
}

}  // namespace paddle::money

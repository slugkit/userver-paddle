#pragma once

#include <paddle/types/enums.hpp>
#include <paddle/types/ids.hpp>
#include <paddle/types/timestamp.hpp>

#include <paddle/types/response.hpp>

#include <userver/logging/log.hpp>

namespace paddle::events {

enum class EventTypeName {
    // Address events
    kAddressCreated,
    kAddressImported,
    kAddressUpdated,
    // Adjustment events
    kAdjustmentCreated,
    kAdjustmentUpdated,
    // API key events
    kApiKeyCreated,
    kApiKeyExpired,
    kApiKeyExpiring,
    kApiKeyRevoked,
    kApiKeyUpdated,
    // Client Token events
    kClientTokenCreated,
    kClientTokenRevoked,
    kClientTokenUpdated,
    // Business events
    kBusinessCreated,
    kBusinessImported,
    kBusinessUpdated,
    // Customer events
    kCustomerCreated,
    kCustomerImported,
    kCustomerUpdated,
    // Discount events
    kDiscountCreated,
    kDiscountImported,
    kDiscountUpdated,
    // Discount Group events
    kDiscountGroupCreated,
    kDiscountGroupUpdated,
    // Payment Method events
    kPaymentMethodSaved,
    kPaymentMethodDeleted,
    // Payout events
    kPayoutCreated,
    kPayoutPaid,
    // Price events
    kPriceCreated,
    kPriceImported,
    kPriceUpdated,
    // Product events
    kProductCreated,
    kProductImported,
    kProductUpdated,
    // Report events
    kReportCreated,
    kReportUpdated,
    // Subscription events
    kSubscriptionActivated,
    kSubscriptionCanceled,
    kSubscriptionCreated,
    kSubscriptionImported,
    kSubscriptionPastDue,
    kSubscriptionPaused,
    kSubscriptionResumed,
    kSubscriptionUpdated,
    kSubscriptionTrialing,
    // Transaction events
    kTransactionBilled,
    kTransactionCanceled,
    kTransactionCompleted,
    kTransactionCreated,
    kTransactionPaid,
    kTransactionPastDue,
    kTransactionPaymentFailed,
    kTransactionReady,
    kTransactionRevised,
    kTransactionUpdated,
};

enum class EventCategory {
    kAddress,
    kAdjustment,
    kApiKey,
    kClientToken,
    kBusiness,
    kCustomer,
    kDiscount,
    kDiscountGroup,
    kPaymentMethod,
    kPayout,
    kPrice,
    kProduct,
    kReport,
    kSubscription,
    kTransaction,
    kUnknown,
};

EventCategory GetEventCategory(EventTypeName event_type);

enum class TrafficSource {
    kPlatform,
    kSimulation,
    kAll,
};

struct EventType {
    EventTypeName name;
    std::string description;
    std::string group;
    std::vector<std::int32_t> available_versions;
};

template <typename T = JSON>
struct Event {
    using PayloadType = T;
    EventId event_id;
    EventTypeName event_type;
    Timestamp occurred_at;
    T data;
};

template <typename T = JSON>
struct EventWithNotification {
    using PayloadType = T;
    EventId event_id;
    EventTypeName event_type;
    Timestamp occurred_at;
    NotificationId notification_id;
    T data;
};

template <EventTypeName TypeName>
using EventPayload = void;

template <typename T>
void LogEventIgnored(const EventWithNotification<T>& event) {
    LOG_INFO() << "Event ignored: " << event.event_id << " " << event.event_type << " " << event.occurred_at
               << " notification_id: " << event.notification_id;
}

template <typename T>
void LogEventIgnored(const Event<T>& event) {
    LOG_INFO() << "Event ignored: " << event.event_id << " " << event.event_type << " " << event.occurred_at;
}

template <typename T>
Event<T> ParsePayload(Event<JSON>&& event) {
    return Event<T>{
        std::move(event.event_id),
        std::move(event.event_type),
        std::move(event.occurred_at),
        event.data.template As<T>()
    };
}

template <typename T>
Event<T> ParsePayload(EventWithNotification<JSON>&& event) {
    return Event<T>{
        std::move(event.event_id),
        std::move(event.event_type),
        std::move(event.occurred_at),
        std::move(event.notification_id),
        event.data.template As<T>()
    };
}

}  // namespace paddle::events

template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::events::EventTypeName>
    : storages::postgres::io::EnumMappingBase<paddle::events::EventTypeName> {
    static constexpr DBTypeName postgres_name = "paddle.event_type_name";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector()
            .Case("address.created", EnumType::kAddressCreated)
            .Case("address.imported", EnumType::kAddressImported)
            .Case("address.updated", EnumType::kAddressUpdated)
            .Case("adjustment.created", EnumType::kAdjustmentCreated)
            .Case("adjustment.updated", EnumType::kAdjustmentUpdated)
            .Case("api_key.created", EnumType::kApiKeyCreated)
            .Case("api_key.expired", EnumType::kApiKeyExpired)
            .Case("api_key.expiring", EnumType::kApiKeyExpiring)
            .Case("api_key.revoked", EnumType::kApiKeyRevoked)
            .Case("api_key.updated", EnumType::kApiKeyUpdated)
            .Case("client_token.created", EnumType::kClientTokenCreated)
            .Case("client_token.revoked", EnumType::kClientTokenRevoked)
            .Case("client_token.updated", EnumType::kClientTokenUpdated)
            .Case("business.created", EnumType::kBusinessCreated)
            .Case("business.imported", EnumType::kBusinessImported)
            .Case("business.updated", EnumType::kBusinessUpdated)
            .Case("customer.created", EnumType::kCustomerCreated)
            .Case("customer.imported", EnumType::kCustomerImported)
            .Case("customer.updated", EnumType::kCustomerUpdated)
            .Case("discount.created", EnumType::kDiscountCreated)
            .Case("discount.imported", EnumType::kDiscountImported)
            .Case("discount.updated", EnumType::kDiscountUpdated)
            .Case("discount_group.created", EnumType::kDiscountGroupCreated)
            .Case("discount_group.updated", EnumType::kDiscountGroupUpdated)
            .Case("payment_method.saved", EnumType::kPaymentMethodSaved)
            .Case("payment_method.deleted", EnumType::kPaymentMethodDeleted)
            .Case("payout.created", EnumType::kPayoutCreated)
            .Case("payout.paid", EnumType::kPayoutPaid)
            .Case("price.created", EnumType::kPriceCreated)
            .Case("price.imported", EnumType::kPriceImported)
            .Case("price.updated", EnumType::kPriceUpdated)
            .Case("product.created", EnumType::kProductCreated)
            .Case("product.imported", EnumType::kProductImported)
            .Case("product.updated", EnumType::kProductUpdated)
            .Case("report.created", EnumType::kReportCreated)
            .Case("report.updated", EnumType::kReportUpdated)
            .Case("subscription.activated", EnumType::kSubscriptionActivated)
            .Case("subscription.canceled", EnumType::kSubscriptionCanceled)
            .Case("subscription.created", EnumType::kSubscriptionCreated)
            .Case("subscription.imported", EnumType::kSubscriptionImported)
            .Case("subscription.past_due", EnumType::kSubscriptionPastDue)
            .Case("subscription.paused", EnumType::kSubscriptionPaused)
            .Case("subscription.resumed", EnumType::kSubscriptionResumed)
            .Case("subscription.updated", EnumType::kSubscriptionUpdated)
            .Case("subscription.trialing", EnumType::kSubscriptionTrialing)
            .Case("transaction.billed", EnumType::kTransactionBilled)
            .Case("transaction.canceled", EnumType::kTransactionCanceled)
            .Case("transaction.completed", EnumType::kTransactionCompleted)
            .Case("transaction.created", EnumType::kTransactionCreated)
            .Case("transaction.paid", EnumType::kTransactionPaid)
            .Case("transaction.past_due", EnumType::kTransactionPastDue)
            .Case("transaction.payment_failed", EnumType::kTransactionPaymentFailed)
            .Case("transaction.ready", EnumType::kTransactionReady)
            .Case("transaction.revised", EnumType::kTransactionRevised)
            .Case("transaction.updated", EnumType::kTransactionUpdated);
    };
};

template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::events::EventCategory>
    : storages::postgres::io::EnumMappingBase<paddle::events::EventCategory> {
    static constexpr DBTypeName postgres_name = "paddle.event_category";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector()
            .Case("address", EnumType::kAddress)
            .Case("adjustment", EnumType::kAdjustment)
            .Case("api_key", EnumType::kApiKey)
            .Case("client_token", EnumType::kClientToken)
            .Case("business", EnumType::kBusiness)
            .Case("customer", EnumType::kCustomer)
            .Case("discount", EnumType::kDiscount)
            .Case("discount_group", EnumType::kDiscountGroup)
            .Case("payment_method", EnumType::kPaymentMethod)
            .Case("payout", EnumType::kPayout)
            .Case("price", EnumType::kPrice)
            .Case("product", EnumType::kProduct)
            .Case("report", EnumType::kReport)
            .Case("subscription", EnumType::kSubscription)
            .Case("transaction", EnumType::kTransaction)
            .Case("unknown", EnumType::kUnknown);
    };
};

template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::events::TrafficSource>
    : storages::postgres::io::EnumMappingBase<paddle::events::TrafficSource> {
    static constexpr DBTypeName postgres_name = "paddle.traffic_source";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector()
            .Case("platform", EnumType::kPlatform)
            .Case("simulation", EnumType::kSimulation)
            .Case("all", EnumType::kAll);
    };
};

namespace paddle::events {

// EventTypeName
template <typename Format>
Format Serialize(const EventTypeName& type, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(type, to);
}

template <typename Value>
EventTypeName Parse(const Value& value, userver::formats::parse::To<EventTypeName> to) {
    return ParseEnum(value, to);
}

inline userver::logging::LogHelper& operator<<(userver::logging::LogHelper& helper, const EventTypeName& type) {
    return LogEnum(helper, type);
}

// EventCategory
template <typename Format>
Format Serialize(const EventCategory& category, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(category, to);
}

template <typename Value>
EventCategory Parse(const Value& value, userver::formats::parse::To<EventCategory> to) {
    return ParseEnum(value, to);
}

inline userver::logging::LogHelper& operator<<(userver::logging::LogHelper& helper, const EventCategory& category) {
    return LogEnum(helper, category);
}

// TrafficSource
template <typename Format>
Format Serialize(const TrafficSource& source, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(source, to);
}

template <typename Value>
TrafficSource Parse(const Value& value, userver::formats::parse::To<TrafficSource> to) {
    return ParseEnum(value, to);
}

// EventType
template <typename Format>
Format Serialize(const EventType& type, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["name"] = type.name;
    builder["description"] = type.description;
    builder["group"] = type.group;
    builder["available_versions"] = type.available_versions;
    return builder.ExtractValue();
}

template <typename Value>
EventType Parse(const Value& value, userver::formats::parse::To<EventType>) {
    EventType type;
    type.name = value["name"].template As<EventTypeName>();
    type.description = value["description"].template As<std::string>();
    type.group = value["group"].template As<std::string>();
    type.available_versions = value["available_versions"].template As<std::vector<std::int32_t>>();
    return type;
}

// Event
template <typename Format, typename T>
Format Serialize(const Event<T>& event, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["event_id"] = event.event_id;
    builder["event_type"] = event.event_type;
    builder["occurred_at"] = event.occurred_at;
    builder["data"] = event.data;
    return builder.ExtractValue();
}

template <typename Value, typename T>
Event<T> Parse(const Value& value, userver::formats::parse::To<Event<T>>) {
    Event<T> event;
    event.event_id = value["event_id"].template As<EventId>();
    event.event_type = value["event_type"].template As<EventTypeName>();
    event.occurred_at = value["occurred_at"].template As<Timestamp>();
    event.data = value["data"].template As<T>();
    return event;
}

// EventWithNotification
template <typename Format, typename T>
Format Serialize(const EventWithNotification<T>& event, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["event_id"] = event.event_id;
    builder["event_type"] = event.event_type;
    builder["occurred_at"] = event.occurred_at;
    builder["notification_id"] = event.notification_id;
    builder["data"] = event.data;
    return builder.ExtractValue();
}

template <typename Value, typename T>
EventWithNotification<T> Parse(const Value& value, userver::formats::parse::To<EventWithNotification<T>>) {
    EventWithNotification<T> event;
    event.event_id = value["event_id"].template As<EventId>();
    event.event_type = value["event_type"].template As<EventTypeName>();
    event.occurred_at = value["occurred_at"].template As<Timestamp>();
    event.notification_id = value["notification_id"].template As<NotificationId>();
    event.data = value["data"].template As<T>();
    return event;
}

}  // namespace paddle::events

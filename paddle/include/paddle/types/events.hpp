#pragma once

#include <paddle/handlers/handle_result.hpp>
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

    EventCategory GetCategory() const {
        return GetEventCategory(name);
    }
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
auto LogEventIgnored(const EventWithNotification<T>& event) -> handlers::HandleResult {
    LOG_INFO() << "Event ignored: " << event.event_id << " " << event.event_type << " " << event.occurred_at
               << " notification_id: " << event.notification_id;
    return handlers::HandleResult{handlers::HandleResultStatus::kIgnored, "Event ignored"};
}

template <typename T>
auto LogEventIgnored(const Event<T>& event) -> handlers::HandleResult {
    LOG_INFO() << "Event ignored: " << event.event_id << " " << event.event_type << " " << event.occurred_at;
    return handlers::HandleResult{handlers::HandleResultStatus::kIgnored, "Event ignored"};
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

namespace paddle {
template <>
constexpr userver::utils::TrivialBiMap kEnumMap<paddle::events::EventTypeName> = [](auto selector) {
    return selector()
        .Case("address.created", events::EventTypeName::kAddressCreated)
        .Case("address.imported", events::EventTypeName::kAddressImported)
        .Case("address.updated", events::EventTypeName::kAddressUpdated)
        .Case("adjustment.created", events::EventTypeName::kAdjustmentCreated)
        .Case("adjustment.updated", events::EventTypeName::kAdjustmentUpdated)
        .Case("api_key.created", events::EventTypeName::kApiKeyCreated)
        .Case("api_key.expired", events::EventTypeName::kApiKeyExpired)
        .Case("api_key.expiring", events::EventTypeName::kApiKeyExpiring)
        .Case("api_key.revoked", events::EventTypeName::kApiKeyRevoked)
        .Case("api_key.updated", events::EventTypeName::kApiKeyUpdated)
        .Case("client_token.created", events::EventTypeName::kClientTokenCreated)
        .Case("client_token.revoked", events::EventTypeName::kClientTokenRevoked)
        .Case("client_token.updated", events::EventTypeName::kClientTokenUpdated)
        .Case("business.created", events::EventTypeName::kBusinessCreated)
        .Case("business.imported", events::EventTypeName::kBusinessImported)
        .Case("business.updated", events::EventTypeName::kBusinessUpdated)
        .Case("customer.created", events::EventTypeName::kCustomerCreated)
        .Case("customer.imported", events::EventTypeName::kCustomerImported)
        .Case("customer.updated", events::EventTypeName::kCustomerUpdated)
        .Case("discount.created", events::EventTypeName::kDiscountCreated)
        .Case("discount.imported", events::EventTypeName::kDiscountImported)
        .Case("discount.updated", events::EventTypeName::kDiscountUpdated)
        .Case("discount_group.created", events::EventTypeName::kDiscountGroupCreated)
        .Case("discount_group.updated", events::EventTypeName::kDiscountGroupUpdated)
        .Case("payment_method.saved", events::EventTypeName::kPaymentMethodSaved)
        .Case("payment_method.deleted", events::EventTypeName::kPaymentMethodDeleted)
        .Case("payout.created", events::EventTypeName::kPayoutCreated)
        .Case("payout.paid", events::EventTypeName::kPayoutPaid)
        .Case("price.created", events::EventTypeName::kPriceCreated)
        .Case("price.imported", events::EventTypeName::kPriceImported)
        .Case("price.updated", events::EventTypeName::kPriceUpdated)
        .Case("product.created", events::EventTypeName::kProductCreated)
        .Case("product.imported", events::EventTypeName::kProductImported)
        .Case("product.updated", events::EventTypeName::kProductUpdated)
        .Case("report.created", events::EventTypeName::kReportCreated)
        .Case("report.updated", events::EventTypeName::kReportUpdated)
        .Case("subscription.activated", events::EventTypeName::kSubscriptionActivated)
        .Case("subscription.canceled", events::EventTypeName::kSubscriptionCanceled)
        .Case("subscription.created", events::EventTypeName::kSubscriptionCreated)
        .Case("subscription.imported", events::EventTypeName::kSubscriptionImported)
        .Case("subscription.past_due", events::EventTypeName::kSubscriptionPastDue)
        .Case("subscription.paused", events::EventTypeName::kSubscriptionPaused)
        .Case("subscription.resumed", events::EventTypeName::kSubscriptionResumed)
        .Case("subscription.updated", events::EventTypeName::kSubscriptionUpdated)
        .Case("subscription.trialing", events::EventTypeName::kSubscriptionTrialing)
        .Case("transaction.billed", events::EventTypeName::kTransactionBilled)
        .Case("transaction.canceled", events::EventTypeName::kTransactionCanceled)
        .Case("transaction.completed", events::EventTypeName::kTransactionCompleted)
        .Case("transaction.created", events::EventTypeName::kTransactionCreated)
        .Case("transaction.paid", events::EventTypeName::kTransactionPaid)
        .Case("transaction.past_due", events::EventTypeName::kTransactionPastDue)
        .Case("transaction.payment_failed", events::EventTypeName::kTransactionPaymentFailed)
        .Case("transaction.ready", events::EventTypeName::kTransactionReady)
        .Case("transaction.revised", events::EventTypeName::kTransactionRevised)
        .Case("transaction.updated", events::EventTypeName::kTransactionUpdated);
};

template <>
constexpr userver::utils::TrivialBiMap kEnumMap<paddle::events::EventCategory> = [](auto selector) {
    return selector()
        .Case("address", events::EventCategory::kAddress)
        .Case("adjustment", events::EventCategory::kAdjustment)
        .Case("api_key", events::EventCategory::kApiKey)
        .Case("client_token", events::EventCategory::kClientToken)
        .Case("business", events::EventCategory::kBusiness)
        .Case("customer", events::EventCategory::kCustomer)
        .Case("discount", events::EventCategory::kDiscount)
        .Case("discount_group", events::EventCategory::kDiscountGroup)
        .Case("payment_method", events::EventCategory::kPaymentMethod)
        .Case("payout", events::EventCategory::kPayout)
        .Case("price", events::EventCategory::kPrice)
        .Case("product", events::EventCategory::kProduct)
        .Case("report", events::EventCategory::kReport)
        .Case("subscription", events::EventCategory::kSubscription)
        .Case("transaction", events::EventCategory::kTransaction)
        .Case("unknown", events::EventCategory::kUnknown);
};

template <>
constexpr userver::utils::TrivialBiMap kEnumMap<paddle::events::TrafficSource> = [](auto selector) {
    return selector()
        .Case("platform", events::TrafficSource::kPlatform)
        .Case("simulation", events::TrafficSource::kSimulation)
        .Case("all", events::TrafficSource::kAll);
};

}  // namespace paddle

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

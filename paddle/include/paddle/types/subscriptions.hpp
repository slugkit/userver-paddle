#pragma once

#include <paddle/types/billing.hpp>
#include <paddle/types/discounts.hpp>
#include <paddle/types/enums.hpp>
#include <paddle/types/ids.hpp>
#include <paddle/types/money.hpp>
#include <paddle/types/price.hpp>
#include <paddle/types/product.hpp>
#include <paddle/types/timestamp.hpp>

#include <paddle/types/response.hpp>

namespace paddle::subscriptions {

struct ScheduledChange {
    ScheduledChangeAction action;
    Timestamp effective_at;
    OptionalTimestamp resume_at;
};

using OptionalScheduledChange = std::optional<ScheduledChange>;

struct ManagementUrls {
    std::optional<std::string> update_payment_method;
    std::string cancel;
};

// TODO template on custom data for price and product
struct Item {
    SubscriptionItemStatus status;
    std::int32_t quantity;
    bool recurring;
    Timestamp created_at;
    Timestamp updated_at;
    OptionalTimestamp previously_billed_at;
    OptionalTimestamp next_billed_at;
    std::optional<TimePeriod> trial_dates;
    prices::JsonPrice price;
    products::JsonProduct product;
};

struct Subscription {
    SubscriptionId id;
    SubscriptionStatus status;
    CustomerId customer_id;
    AddressId address_id;
    OptionalBusinessId business_id;
    money::CurrencyCode currency_code;
    Timestamp created_at;
    Timestamp updated_at;
    OptionalTimestamp started_at;
    OptionalTimestamp first_billed_at;
    OptionalTimestamp next_billed_at;
    OptionalTimestamp paused_at;
    OptionalTimestamp canceled_at;
    OptionalDiscountSubscription discount;
    CollectionMode collection_mode;
    OptionalBillingDetails billing_details;
    TimePeriod current_billing_period;
    Duration billing_cycle;
    OptionalScheduledChange scheduled_change;
    ManagementUrls management_urls;
    std::vector<Item> items;
    JSON external_data;
    // there's import meta, but we don't care about it
};

}  // namespace paddle::subscriptions

namespace paddle::subscriptions {

// ScheduledChange
template <typename Format>
Format Serialize(const ScheduledChange& scheduled_change, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["action"] = scheduled_change.action;
    builder["effective_at"] = scheduled_change.effective_at;
    builder["resume_at"] = scheduled_change.resume_at;
    return builder.ExtractValue();
}

template <typename Value>
ScheduledChange Parse(const Value& value, userver::formats::parse::To<ScheduledChange>) {
    ScheduledChange scheduled_change;
    scheduled_change.action = value["action"].template As<ScheduledChangeAction>();
    scheduled_change.effective_at = value["effective_at"].template As<Timestamp>();
    scheduled_change.resume_at = value["resume_at"].template As<OptionalTimestamp>();
    return scheduled_change;
}

// ManagementUrls
template <typename Format>
Format Serialize(const ManagementUrls& management_urls, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["update_payment_method"] = management_urls.update_payment_method;
    builder["cancel"] = management_urls.cancel;
    return builder.ExtractValue();
}

template <typename Value>
ManagementUrls Parse(const Value& value, userver::formats::parse::To<ManagementUrls>) {
    ManagementUrls management_urls;
    management_urls.update_payment_method = value["update_payment_method"].template As<std::optional<std::string>>();
    management_urls.cancel = value["cancel"].template As<std::string>();
    return management_urls;
}

// ItemSubscription

template <typename Format>
Format Serialize(const Item& item_subscription, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["status"] = item_subscription.status;
    builder["quantity"] = item_subscription.quantity;
    builder["recurring"] = item_subscription.recurring;
    builder["created_at"] = item_subscription.created_at;
    builder["updated_at"] = item_subscription.updated_at;
    builder["previously_billed_at"] = item_subscription.previously_billed_at;
    builder["next_billed_at"] = item_subscription.next_billed_at;
    builder["trial_dates"] = item_subscription.trial_dates;
    builder["price"] = item_subscription.price;
    builder["product"] = item_subscription.product;
    return builder.ExtractValue();
}

template <typename Value>
Item Parse(const Value& value, userver::formats::parse::To<Item>) {
    Item item_subscription;
    item_subscription.status = value["status"].template As<SubscriptionItemStatus>();
    item_subscription.quantity = value["quantity"].template As<std::int32_t>();
    item_subscription.recurring = value["recurring"].template As<bool>();
    item_subscription.created_at = value["created_at"].template As<Timestamp>();
    item_subscription.updated_at = value["updated_at"].template As<Timestamp>();
    item_subscription.previously_billed_at = value["previously_billed_at"].template As<OptionalTimestamp>();
    item_subscription.next_billed_at = value["next_billed_at"].template As<OptionalTimestamp>();
    if (value.HasMember("trial_dates") && !value["trial_dates"].IsNull()) {
        item_subscription.trial_dates = value["trial_dates"].template As<TimePeriod>();
    }
    item_subscription.price = value["price"].template As<prices::JsonPrice>();
    item_subscription.product = value["product"].template As<products::JsonProduct>();
    return item_subscription;
}

// Subscription
template <typename Format>
Format Serialize(const Subscription& subscription, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["id"] = subscription.id;
    builder["status"] = subscription.status;
    builder["customer_id"] = subscription.customer_id;
    builder["address_id"] = subscription.address_id;
    builder["business_id"] = subscription.business_id;
    builder["currency_code"] = subscription.currency_code;
    builder["created_at"] = subscription.created_at;
    builder["updated_at"] = subscription.updated_at;
    builder["started_at"] = subscription.started_at;
    builder["first_billed_at"] = subscription.first_billed_at;
    builder["next_billed_at"] = subscription.next_billed_at;
    builder["paused_at"] = subscription.paused_at;
    builder["canceled_at"] = subscription.canceled_at;
    builder["discount"] = subscription.discount;
    builder["collection_mode"] = subscription.collection_mode;
    builder["billing_details"] = subscription.billing_details;
    builder["current_billing_period"] = subscription.current_billing_period;
    builder["billing_cycle"] = subscription.billing_cycle;
    builder["scheduled_change"] = subscription.scheduled_change;
    builder["management_urls"] = subscription.management_urls;
    builder["items"] = subscription.items;
    builder["external_data"] = subscription.external_data;
    return builder.ExtractValue();
}

template <typename Value>
Subscription Parse(const Value& value, userver::formats::parse::To<Subscription>) {
    Subscription subscription;
    subscription.id = value["id"].template As<SubscriptionId>();
    subscription.status = value["status"].template As<SubscriptionStatus>();
    subscription.customer_id = value["customer_id"].template As<CustomerId>();
    subscription.address_id = value["address_id"].template As<AddressId>();
    subscription.business_id = value["business_id"].template As<OptionalBusinessId>();
    subscription.currency_code = value["currency_code"].template As<money::CurrencyCode>();
    subscription.created_at = value["created_at"].template As<Timestamp>();
    subscription.updated_at = value["updated_at"].template As<Timestamp>();
    subscription.started_at = value["started_at"].template As<OptionalTimestamp>();
    subscription.first_billed_at = value["first_billed_at"].template As<OptionalTimestamp>();
    subscription.next_billed_at = value["next_billed_at"].template As<OptionalTimestamp>();
    subscription.paused_at = value["paused_at"].template As<OptionalTimestamp>();
    subscription.canceled_at = value["canceled_at"].template As<OptionalTimestamp>();
    subscription.discount = value["discount"].template As<OptionalDiscountSubscription>();
    subscription.collection_mode = value["collection_mode"].template As<CollectionMode>();
    subscription.billing_details = value["billing_details"].template As<OptionalBillingDetails>();
    subscription.current_billing_period = value["current_billing_period"].template As<TimePeriod>();
    subscription.billing_cycle = value["billing_cycle"].template As<Duration>();
    subscription.scheduled_change = value["scheduled_change"].template As<OptionalScheduledChange>();
    if (value.HasMember("management_urls")) {
        subscription.management_urls = value["management_urls"].template As<ManagementUrls>();
    }
    subscription.items = value["items"].template As<std::vector<Item>>();
    subscription.external_data = value["external_data"].template As<JSON>();
    return subscription;
}

}  // namespace paddle::subscriptions
#pragma once

#include <paddle/types/ids.hpp>
#include <paddle/types/timestamp.hpp>

#include <paddle/types/formats.hpp>

namespace paddle {

struct DiscountSubscription {
    DiscountId id;
    OptionalTimestamp starts_at;
    OptionalTimestamp ends_at;
};

using OptionalDiscountSubscription = std::optional<DiscountSubscription>;

}  // namespace paddle

namespace paddle {

// DiscountSubscription

template <typename Format>
Format Serialize(const DiscountSubscription& subscription, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["id"] = subscription.id;
    builder["starts_at"] = subscription.starts_at;
    builder["ends_at"] = subscription.ends_at;
    return builder.ExtractValue();
}

template <typename Value>
DiscountSubscription Parse(const Value& value, userver::formats::parse::To<DiscountSubscription>) {
    DiscountSubscription subscription;
    subscription.id = value["id"].template As<DiscountId>();
    subscription.starts_at = value["starts_at"].template As<OptionalTimestamp>();
    subscription.ends_at = value["ends_at"].template As<OptionalTimestamp>();
    return subscription;
}

}  // namespace paddle
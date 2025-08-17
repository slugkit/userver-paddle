#pragma once

#include <paddle/types/formats.hpp>

#include <userver/storages/postgres/io/chrono.hpp>

namespace paddle {

using Timestamp = userver::storages::postgres::TimePointTz;
using OptionalTimestamp = std::optional<Timestamp>;

struct TimePeriod {
    Timestamp starts_at;
    Timestamp ends_at;
};

}  // namespace paddle

namespace paddle {

// TimePeriod
template <typename Format>
Format Serialize(const TimePeriod& time_period, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["starts_at"] = time_period.starts_at;
    builder["ends_at"] = time_period.ends_at;
    return builder.ExtractValue();
}

template <typename Value>
TimePeriod Parse(const Value& value, userver::formats::parse::To<TimePeriod>) {
    TimePeriod time_period;
    time_period.starts_at = value["starts_at"].template As<Timestamp>();
    time_period.ends_at = value["ends_at"].template As<Timestamp>();
    return time_period;
}

}  // namespace paddle
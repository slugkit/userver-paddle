#pragma once

#include <paddle/types/enums.hpp>
#include <paddle/types/formats.hpp>

#include <cstdint>

namespace paddle {

struct Duration {
    Interval interval;
    std::int32_t frequency;
};

using OptionalDuration = std::optional<Duration>;

}  // namespace paddle

namespace paddle {

// Duration
template <typename Format>
Format Serialize(const Duration& duration, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["interval"] = duration.interval;
    builder["frequency"] = duration.frequency;
    return builder.ExtractValue();
}

template <typename Value>
Duration Parse(const Value& value, userver::formats::parse::To<Duration>) {
    Duration duration;
    duration.interval = value["interval"].template As<Interval>();
    duration.frequency = value["frequency"].template As<std::int32_t>();
    return duration;
}

}  // namespace paddle

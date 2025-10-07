#pragma once

#include <userver/formats/parse/boost_optional.hpp>
#include <userver/formats/parse/boost_uuid.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/parse/to.hpp>
#include <userver/formats/serialize/boost_optional.hpp>
#include <userver/formats/serialize/boost_uuid.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/formats/serialize/to.hpp>

#include <userver/formats/json.hpp>

namespace paddle {

using JSON = userver::formats::json::Value;

template <typename T>
concept FormatType = requires(T) { typename T::Builder; };

template <typename T, typename Format>
concept SerializableTo = requires(T t) { Serialize(t, userver::formats::serialize::To<Format>{}); };

template <typename T, typename Format>
concept ParseableFrom = requires(T t) { Parse(std::declval<Format>(), userver::formats::parse::To<T>{}); };

}  // namespace paddle

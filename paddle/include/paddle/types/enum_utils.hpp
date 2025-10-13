#pragma once

#include <paddle/types/formats.hpp>

#include <userver/logging/log_helper.hpp>
#include <userver/utils/trivial_map.hpp>

#include <string>
#include <type_traits>

namespace paddle {

template <typename Enum>
requires std::is_enum_v<Enum>
constexpr userver::utils::TrivialBiMap kEnumMap = [](auto) { static_assert(false, "EnumMap is not provided"); };

template <typename Enum>
concept HasEnumMap = requires { kEnumMap<Enum>; };

template <typename Enum>
requires HasEnumMap<Enum>
auto EnumToString(const Enum& enum_value) -> std::string {
    using EnumUnderlyingType = std::underlying_type_t<Enum>;
    auto literal_ptr = kEnumMap<Enum>.TryFind(enum_value);
    if (literal_ptr.has_value()) {
        return std::string(*literal_ptr);
    }
    throw std::invalid_argument(fmt::format("Invalid enum value: {}", static_cast<EnumUnderlyingType>(enum_value)));
}

template <typename Enum>
requires HasEnumMap<Enum>
auto EnumFromString(const std::string& str) -> Enum {
    auto enum_value_ptr = kEnumMap<Enum>.TryFind(str);
    if (enum_value_ptr.has_value()) {
        return *enum_value_ptr;
    }
    throw std::invalid_argument(fmt::format("Invalid enum value: {}", str));
}

template <typename Enum, typename Format>
requires HasEnumMap<Enum>
auto SerializeEnum(const Enum& enum_value, userver::formats::serialize::To<Format>) -> Format {
    typename Format::Builder builder(EnumToString(enum_value));
    return builder.ExtractValue();
}

template <typename Enum, typename Value>
requires(!std::is_same_v<Value, std::string> && HasEnumMap<Enum>)
auto ParseEnum(const Value& value, userver::formats::parse::To<Enum>) -> Enum {
    auto enum_value = value.template As<std::string>();
    return EnumFromString<Enum>(enum_value);
}

template <typename Enum>
requires HasEnumMap<Enum>
auto LogEnum(userver::logging::LogHelper& helper, const Enum& enum_value) -> userver::logging::LogHelper& {
    return helper << EnumToString(enum_value);
}

template <typename Enum>
requires HasEnumMap<Enum>
auto Parse(const std::string& str, userver::formats::parse::To<Enum>) -> Enum {
    return EnumFromString<Enum>(str);
}

template <typename Format, typename Enum>
requires HasEnumMap<Enum>
auto Serialize(const Enum& enum_value, userver::formats::serialize::To<Format> to) -> Format {
    return SerializeEnum(enum_value, to);
}

template <typename Value, typename Enum>
requires(!std::is_same_v<std::decay_t<Value>, std::string> && HasEnumMap<Enum>)
auto Parse(const Value& value, userver::formats::parse::To<Enum> to) -> Enum {
    return ParseEnum(value, to);
}

template <typename Enum>
requires HasEnumMap<Enum>
auto ToString(const Enum& enum_value) -> std::string {
    return EnumToString(enum_value);
}

template <typename Enum>
requires HasEnumMap<Enum>
auto operator<<(userver::logging::LogHelper& helper, const Enum& enum_value) -> userver::logging::LogHelper& {
    return LogEnum(helper, enum_value);
}

}  // namespace paddle
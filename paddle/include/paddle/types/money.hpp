#pragma once

#include <paddle/types/formats.hpp>

#include <cstdint>
#include <string>

namespace paddle::money {

using CurrencyCode = userver::utils::StrongTypedef<struct CurrencyCodeTag, std::string>;

struct Money {
    std::int32_t amount;
    CurrencyCode currency_code;

    bool operator==(const Money& other) const {
        return amount == other.amount && currency_code == other.currency_code;
    }

    bool operator!=(const Money& other) const {
        return !(*this == other);
    }
};

}  // namespace paddle::money

namespace paddle::money {

template <typename Format>
Format Serialize(const Money& money, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["amount"] = std::to_string(money.amount);
    builder["currency_code"] = money.currency_code;
    return builder.ExtractValue();
}

template <typename Value>
Money Parse(const Value& value, userver::formats::parse::To<Money>) {
    Money money;
    money.amount = std::stoi(value["amount"].template As<std::string>());
    money.currency_code = value["currency_code"].template As<CurrencyCode>();
    return money;
}

}  // namespace paddle::money

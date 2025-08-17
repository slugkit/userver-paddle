#pragma once

#include <paddle/types/duration.hpp>
#include <paddle/types/money.hpp>
#include <paddle/types/response.hpp>
#include <paddle/types/timestamp.hpp>

namespace paddle::prices {

using CountryCode = userver::utils::StrongTypedef<struct CountryCodeTag, std::string>;

struct PriceOverride {
    std::vector<CountryCode> country_codes;
    money::Money unit_price;
};

struct PriceQuantity {
    std::int32_t minimum;
    std::int32_t maximum;
};

struct Price {
    using IdType = PriceId;

    PriceId id;
    ProductId product_id;
    std::string description;
    CatalogType type;
    std::string name;
    OptionalDuration billing_cycle;
    OptionalDuration trial_period;
    std::string tax_mode;  // it is actually an enum, but we don't care
    money::Money unit_price;
    std::vector<PriceOverride> unit_price_overrides;
    PriceQuantity quantity;
    Status status;
    JSON custom_data;
    // there's import_meta field, but we don't care about it
    Timestamp created_at;
    Timestamp updated_at;
};

using PriceResponse = Response<Price, MetaPaginated>;

}  // namespace paddle::prices

namespace paddle::prices {

template <typename Format>
Format Serialize(const PriceOverride& override, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["country_codes"] = override.country_codes;
    builder["unit_price"] = override.unit_price;
    return builder.ExtractValue();
}

template <typename Value>
PriceOverride Parse(const Value& value, userver::formats::parse::To<PriceOverride>) {
    PriceOverride override;
    override.country_codes = value["country_codes"].template As<std::vector<CountryCode>>();
    override.unit_price = value["unit_price"].template As<money::Money>();
    return override;
}

template <typename Format>
Format Serialize(const PriceQuantity& quantity, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["minimum"] = quantity.minimum;
    builder["maximum"] = quantity.maximum;
    return builder.ExtractValue();
}

template <typename Value>
PriceQuantity Parse(const Value& value, userver::formats::parse::To<PriceQuantity>) {
    PriceQuantity quantity;
    quantity.minimum = value["minimum"].template As<std::int32_t>();
    quantity.maximum = value["maximum"].template As<std::int32_t>();
    return quantity;
}

template <typename Format>
Format Serialize(const Price& price, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["id"] = price.id;
    builder["product_id"] = price.product_id;
    builder["description"] = price.description;
    builder["type"] = price.type;
    builder["name"] = price.name;
    builder["billing_cycle"] = price.billing_cycle;
    builder["trial_period"] = price.trial_period;
    builder["tax_mode"] = price.tax_mode;
    builder["unit_price"] = price.unit_price;
    builder["unit_price_overrides"] = price.unit_price_overrides;
    builder["quantity"] = price.quantity;
    builder["status"] = price.status;
    builder["custom_data"] = price.custom_data;
    builder["created_at"] = price.created_at;
    builder["updated_at"] = price.updated_at;
    return builder.ExtractValue();
}

template <typename Value>
Price Parse(const Value& value, userver::formats::parse::To<Price>) {
    Price price;
    price.id = value["id"].template As<PriceId>();
    price.product_id = value["product_id"].template As<ProductId>();
    price.description = value["description"].template As<std::string>();
    price.type = value["type"].template As<CatalogType>();
    price.name = value["name"].template As<std::string>();
    if (value.HasMember("billing_cycle")) {
        price.billing_cycle = value["billing_cycle"].template As<OptionalDuration>();
    }
    if (value.HasMember("trial_period")) {
        price.trial_period = value["trial_period"].template As<OptionalDuration>();
    }
    price.tax_mode = value["tax_mode"].template As<std::string>();
    price.unit_price = value["unit_price"].template As<money::Money>();
    price.unit_price_overrides = value["unit_price_overrides"].template As<std::vector<PriceOverride>>();
    price.quantity = value["quantity"].template As<PriceQuantity>();
    price.status = value["status"].template As<Status>();
    price.custom_data = value["custom_data"].template As<userver::formats::json::Value>();
    price.created_at = value["created_at"].template As<Timestamp>();
    price.updated_at = value["updated_at"].template As<Timestamp>();
    return price;
}

}  // namespace paddle::prices
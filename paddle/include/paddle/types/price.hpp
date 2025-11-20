#pragma once

#include <paddle/types/convert.hpp>
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

template <typename CustomData = JSON>
struct PriceTemplate {
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
    CustomData custom_data;
    // there's import_meta field, but we don't care about it
    Timestamp created_at;
    Timestamp updated_at;

    template <typename T>
    requires(std::is_same_v<JSON, CustomData>)
    auto GetCustomData() const -> T {
        return custom_data.template As<T>();
    }
    template <typename T>
    requires(!std::is_same_v<JSON, CustomData> && std::is_same_v<T, CustomData>)
    auto GetCustomData() const -> const T& {
        return custom_data;
    }
};

using JsonPrice = PriceTemplate<>;

template <typename CustomData = JSON>
using PriceResponse = Response<PriceTemplate<CustomData>, MetaPaginated>;

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

template <typename Format, typename CustomData>
Format Serialize(const PriceTemplate<CustomData>& price, userver::formats::serialize::To<Format>) {
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

template <typename Value, typename CustomData>
PriceTemplate<CustomData> Parse(const Value& value, userver::formats::parse::To<PriceTemplate<CustomData>>) {
    PriceTemplate<CustomData> price;
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
    price.custom_data = value["custom_data"].template As<CustomData>();
    price.created_at = value["created_at"].template As<Timestamp>();
    price.updated_at = value["updated_at"].template As<Timestamp>();
    return price;
}

namespace impl {

template <typename From, typename To>
struct PriceConverter;

// Identity converter
template <typename PricePayload>
struct PriceConverter<PricePayload, PricePayload> {
    static auto Convert(const PriceTemplate<PricePayload>& price) -> PriceTemplate<PricePayload> {
        return price;
    }

    static auto Convert(PriceTemplate<PricePayload>&& price) -> PriceTemplate<PricePayload> {
        return std::move(price);
    }
};

// Serializing converter
template <typename From, FormatType To>
requires SerializableTo<From, To>
struct PriceConverter<From, To> {
    static auto Convert(const PriceTemplate<From>& price) -> PriceTemplate<To> {
        PriceTemplate<To> result;
        result.id = price.id;
        result.product_id = price.product_id;
        result.description = price.description;
        result.type = price.type;
        result.billing_cycle = price.billing_cycle;
        result.trial_period = price.trial_period;
        result.tax_mode = price.tax_mode;
        result.unit_price = price.unit_price;
        result.unit_price_overrides = price.unit_price_overrides;
        result.quantity = price.quantity;
        result.status = price.status;
        result.custom_data = Serialize(price.custom_data, userver::formats::serialize::To<To>{});
        result.created_at = price.created_at;
        result.updated_at = price.updated_at;
        return result;
    }
};

// Parsing converter
template <FormatType From, typename To>
requires ParseableFrom<To, From>
struct PriceConverter<From, To> {
    static auto Convert(const PriceTemplate<From>& price) -> PriceTemplate<To> {
        auto custom_data = Parse(price.custom_data, userver::formats::parse::To<To>{});
        PriceTemplate<To> result;
        result.id = price.id;
        result.product_id = price.product_id;
        result.description = price.description;
        result.type = price.type;
        result.billing_cycle = price.billing_cycle;
        result.trial_period = price.trial_period;
        result.tax_mode = price.tax_mode;
        result.unit_price = price.unit_price;
        result.unit_price_overrides = price.unit_price_overrides;
        result.quantity = price.quantity;
        result.status = price.status;
        result.custom_data = custom_data;
        result.created_at = price.created_at;
        result.updated_at = price.updated_at;
        return result;
    }
};

}  // namespace impl

template <typename From, FormatType Format>
requires SerializableTo<From, Format>
auto Convert(const PriceTemplate<From>& price, convert::To<PriceTemplate<Format>>) -> PriceTemplate<Format> {
    return impl::PriceConverter<From, Format>::Convert(price);
}

template <FormatType Format, typename To>
requires ParseableFrom<To, Format>
auto Convert(const PriceTemplate<Format>& price, convert::To<PriceTemplate<To>>) -> PriceTemplate<To> {
    return impl::PriceConverter<Format, To>::Convert(price);
}

template <typename T>
auto Convert(const PriceTemplate<T>& price, convert::To<PriceTemplate<T>>) -> PriceTemplate<T> {
    return impl::PriceConverter<T, T>::Convert(price);
}

template <typename To, typename From>
auto Convert(const PriceTemplate<From>& price) -> PriceTemplate<To> {
    return Convert(price, convert::To<PriceTemplate<To>>{});
}

}  // namespace paddle::prices
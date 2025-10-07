#pragma once

#include <paddle/types/discounts.hpp>
#include <paddle/types/ids.hpp>
#include <paddle/types/price.hpp>
#include <paddle/types/product.hpp>

namespace paddle::prices {

struct Totals {
    std::int32_t subtotal;
    std::int32_t discount;
    std::int32_t tax;
    std::int32_t total;
};

struct FormattedTotals {
    std::string subtotal;
    std::string discount;
    std::string tax;
    std::string total;
};

struct Address {
    std::string country_code;
    std::optional<std::string> postal_code;
};

template <typename ProductData, typename PriceData>
struct ItemTemplate {
    using ProductType = products::ProductTemplate<ProductData>;
    using PriceType = PriceTemplate<PriceData>;

    ProductType product;
    PriceType price;
    std::int32_t quantity;
    std::string tax_rate;
    Totals unit_totals;
    FormattedTotals formatted_unit_totals;
    Totals totals;
    FormattedTotals formatted_totals;
    JSON discounts;
};

template <typename ProductData = JSON, typename PriceData = JSON>
struct DetailsTemplate {
    using ItemType = ItemTemplate<ProductData, PriceData>;
    std::vector<ItemType> line_items;
};

template <typename ProductData = JSON, typename PriceData = JSON>
struct PricePreviewTemplate {
    using ItemType = ItemTemplate<ProductData, PriceData>;
    OptionalCustomerId customer_id;
    OptionalAddressId address_id;
    OptionalBusinessId business_id;
    money::CurrencyCode currency_code;
    OptionalDiscountId discount_id;
    Address address;
    std::optional<std::string> customer_ip_address;
    DetailsTemplate<ProductData, PriceData> details;
    std::vector<PaymentMethodType> available_payment_methods;
};

struct PricePreviewRequestItem {
    PriceId price_id;
    std::int32_t quantity;
};

struct PricePreviewRequest {
    std::vector<PricePreviewRequestItem> items;
    OptionalCustomerId customer_id;
    OptionalAddressId address_id;
    OptionalBusinessId business_id;
    std::optional<money::CurrencyCode> currency_code;
    OptionalDiscountId discount_id;
    Address address;
    std::optional<std::string> customer_ip_address;
};

using JsonPricePreview = PricePreviewTemplate<JSON, JSON>;

template <typename Format>
auto Serialize(const Totals& totals, userver::formats::serialize::To<Format>) -> Format {
    typename Format::Builder builder;
    builder["subtotal"] = std::to_string(totals.subtotal);
    builder["discount"] = std::to_string(totals.discount);
    builder["tax"] = std::to_string(totals.tax);
    builder["total"] = std::to_string(totals.total);
    return builder.ExtractValue();
}

template <typename Value>
auto Parse(const Value& value, userver::formats::parse::To<Totals>) -> Totals {
    Totals totals;
    totals.subtotal = std::stoi(value["subtotal"].template As<std::string>());
    totals.discount = std::stoi(value["discount"].template As<std::string>());
    totals.tax = std::stoi(value["tax"].template As<std::string>());
    totals.total = std::stoi(value["total"].template As<std::string>());
    return totals;
}

template <typename Format>
auto Serialize(const FormattedTotals& formatted_totals, userver::formats::serialize::To<Format>) -> Format {
    typename Format::Builder builder;
    builder["subtotal"] = formatted_totals.subtotal;
    builder["discount"] = formatted_totals.discount;
    builder["tax"] = formatted_totals.tax;
    builder["total"] = formatted_totals.total;
    return builder.ExtractValue();
}

template <typename Value>
auto Parse(const Value& value, userver::formats::parse::To<FormattedTotals>) -> FormattedTotals {
    FormattedTotals formatted_totals;
    formatted_totals.subtotal = value["subtotal"].template As<std::string>();
    formatted_totals.discount = value["discount"].template As<std::string>();
    formatted_totals.tax = value["tax"].template As<std::string>();
    formatted_totals.total = value["total"].template As<std::string>();
    return formatted_totals;
}

template <typename Format>
auto Serialize(const Address& address, userver::formats::serialize::To<Format>) -> Format {
    typename Format::Builder builder;
    builder["country_code"] = address.country_code;
    builder["postal_code"] = address.postal_code;
    return builder.ExtractValue();
}

template <typename Value>
auto Parse(const Value& value, userver::formats::parse::To<Address>) -> Address {
    Address address;
    address.country_code = value["country_code"].template As<std::string>();
    address.postal_code = value["postal_code"].template As<std::optional<std::string>>();
    return address;
}

template <typename ProductData, typename PriceData, typename Format>
auto Serialize(const ItemTemplate<ProductData, PriceData>& item, userver::formats::serialize::To<Format>) -> Format {
    typename Format::Builder builder;
    builder["product"] = item.product;
    builder["price"] = item.price;
    builder["quantity"] = item.quantity;
    builder["tax_rate"] = item.tax_rate;
    builder["unit_totals"] = item.unit_totals;
    builder["formatted_unit_totals"] = item.formatted_unit_totals;
    builder["totals"] = item.totals;
    builder["formatted_totals"] = item.formatted_totals;
    builder["discounts"] = item.discounts;
    return builder.ExtractValue();
}

template <typename Value, typename ProductData, typename PriceData>
auto Parse(const Value& value, userver::formats::parse::To<ItemTemplate<ProductData, PriceData>>)
    -> ItemTemplate<ProductData, PriceData> {
    using ItemType = ItemTemplate<ProductData, PriceData>;
    ItemType item;
    item.product = value["product"].template As<typename ItemType::ProductType>();
    item.price = value["price"].template As<typename ItemType::PriceType>();
    item.quantity = value["quantity"].template As<std::int32_t>();
    item.tax_rate = value["tax_rate"].template As<std::string>();
    item.unit_totals = value["unit_totals"].template As<Totals>();
    item.formatted_unit_totals = value["formatted_unit_totals"].template As<FormattedTotals>();
    item.totals = value["totals"].template As<Totals>();
    item.formatted_totals = value["formatted_totals"].template As<FormattedTotals>();
    item.discounts = value["discounts"].template As<JSON>();
    return item;
}

template <typename ProductData, typename PriceData, typename Format>
auto Serialize(const DetailsTemplate<ProductData, PriceData>& details, userver::formats::serialize::To<Format>)
    -> Format {
    typename Format::Builder builder;
    builder["line_items"] = details.line_items;
    return builder.ExtractValue();
}

template <typename Value, typename ProductData, typename PriceData>
auto Parse(const Value& value, userver::formats::parse::To<DetailsTemplate<ProductData, PriceData>>)
    -> DetailsTemplate<ProductData, PriceData> {
    using DetailsType = DetailsTemplate<ProductData, PriceData>;
    using ItemType = typename DetailsType::ItemType;
    DetailsType details;
    details.line_items = value["line_items"].template As<std::vector<ItemType>>();
    return details;
}

template <typename ProductData, typename PriceData, typename Format>
auto Serialize(const PricePreviewTemplate<ProductData, PriceData>& price_preview, userver::formats::serialize::To<Format>)
    -> Format {
    typename Format::Builder builder;
    builder["customer_id"] = price_preview.customer_id;
    builder["address_id"] = price_preview.address_id;
    builder["business_id"] = price_preview.business_id;
    builder["currency_code"] = price_preview.currency_code;
    builder["discount_id"] = price_preview.discount_id;
    builder["address"] = price_preview.address;
    builder["customer_ip_address"] = price_preview.customer_ip_address;
    builder["details"] = price_preview.details;
    builder["available_payment_methods"] = price_preview.available_payment_methods;
    return builder.ExtractValue();
}

template <typename ProductData, typename PriceData, typename Value>
auto Parse(const Value& value, userver::formats::parse::To<PricePreviewTemplate<ProductData, PriceData>>)
    -> PricePreviewTemplate<ProductData, PriceData> {
    PricePreviewTemplate<ProductData, PriceData> price_preview;
    price_preview.customer_id = value["customer_id"].template As<OptionalCustomerId>();
    price_preview.address_id = value["address_id"].template As<OptionalAddressId>();
    price_preview.business_id = value["business_id"].template As<OptionalBusinessId>();
    price_preview.currency_code = value["currency_code"].template As<money::CurrencyCode>();
    price_preview.discount_id = value["discount_id"].template As<OptionalDiscountId>();
    price_preview.address = value["address"].template As<Address>();
    price_preview.customer_ip_address = value["customer_ip_address"].template As<std::optional<std::string>>();
    price_preview.details = value["details"].template As<DetailsTemplate<ProductData, PriceData>>();
    price_preview.available_payment_methods =
        value["available_payment_methods"].template As<std::vector<PaymentMethodType>>();
    return price_preview;
}

template <typename Format>
auto Serialize(const PricePreviewRequestItem& item, userver::formats::serialize::To<Format>) -> Format {
    typename Format::Builder builder;
    builder["price_id"] = item.price_id;
    builder["quantity"] = item.quantity;
    return builder.ExtractValue();
}

template <typename Value>
auto Parse(const Value& value, userver::formats::parse::To<PricePreviewRequestItem>) -> PricePreviewRequestItem {
    PricePreviewRequestItem item;
    item.price_id = value["price_id"].template As<PriceId>();
    item.quantity = value["quantity"].template As<std::int32_t>();
    return item;
}

template <typename Format>
auto Serialize(const PricePreviewRequest& request, userver::formats::serialize::To<Format>) -> Format {
    typename Format::Builder builder;
    builder["items"] = request.items;
    builder["customer_id"] = request.customer_id;
    builder["address_id"] = request.address_id;
    builder["business_id"] = request.business_id;
    builder["currency_code"] = request.currency_code;
    builder["discount_id"] = request.discount_id;
    builder["address"] = request.address;
    builder["customer_ip_address"] = request.customer_ip_address;
    return builder.ExtractValue();
}

template <typename Value>
auto Parse(const Value& value, userver::formats::parse::To<PricePreviewRequest>) -> PricePreviewRequest {
    PricePreviewRequest request;
    request.items = value["items"].template As<std::vector<PricePreviewRequestItem>>();
    request.customer_id = value["customer_id"].template As<OptionalCustomerId>();
    request.address_id = value["address_id"].template As<OptionalAddressId>();
    request.business_id = value["business_id"].template As<OptionalBusinessId>();
    request.currency_code = value["currency_code"].template As<std::optional<money::CurrencyCode>>();
    request.discount_id = value["discount_id"].template As<OptionalDiscountId>();
    request.address = value["address"].template As<Address>();
    request.customer_ip_address = value["customer_ip_address"].template As<std::optional<std::string>>();
    return request;
}

namespace impl {

//-------------------------------------------
// PriceItemPreviewConverter
//-------------------------------------------
template <typename From, typename To>
struct PriceItemPreviewConverter;

// Identity converter
template <typename ProductData, typename PriceData>
struct PriceItemPreviewConverter<ItemTemplate<ProductData, PriceData>, ItemTemplate<ProductData, PriceData>> {
    static auto DoConvert(const ItemTemplate<ProductData, PriceData>& item) -> ItemTemplate<ProductData, PriceData> {
        return item;
    }

    static auto DoConvert(ItemTemplate<ProductData, PriceData>&& item) -> ItemTemplate<ProductData, PriceData> {
        return std::move(item);
    }
};

// Parsing converter
template <typename FromProductData, typename FromPriceData, typename ToProductData, typename ToPriceData>
struct PriceItemPreviewConverter<
    ItemTemplate<FromProductData, FromPriceData>,
    ItemTemplate<ToProductData, ToPriceData>> {
    static auto DoConvert(const ItemTemplate<FromProductData, FromPriceData>& price_preview
    ) -> ItemTemplate<ToProductData, ToPriceData> {
        using ItemType = ItemTemplate<ToProductData, ToPriceData>;
        using ProductType = typename ItemType::ProductType;
        using PriceType = typename ItemType::PriceType;
        ItemType item;
        item.product = Convert(price_preview.product, convert::To<ProductType>{});
        item.price = Convert(price_preview.price, convert::To<PriceType>{});
        item.quantity = price_preview.quantity;
        item.tax_rate = price_preview.tax_rate;
        item.unit_totals = price_preview.unit_totals;
        item.formatted_unit_totals = price_preview.formatted_unit_totals;
        item.totals = price_preview.totals;
        item.formatted_totals = price_preview.formatted_totals;
        item.discounts = price_preview.discounts;
        return item;
    }
};

}  // namespace impl

template <typename FromProductData, typename FromPriceData, typename ToProductData, typename ToPriceData>
auto Convert(const ItemTemplate<FromProductData, FromPriceData>& item, convert::To<ItemTemplate<ToProductData, ToPriceData>>)
    -> ItemTemplate<ToProductData, ToPriceData> {
    return impl::PriceItemPreviewConverter<
        ItemTemplate<FromProductData, FromPriceData>,
        ItemTemplate<ToProductData, ToPriceData>>::DoConvert(item);
}

namespace impl {
//-------------------------------------------
// PricePreviewConverter
//-------------------------------------------
template <typename From, typename To>
struct PricePreviewConverter;

// Identity converter
template <typename ProductData, typename PriceData>
struct PricePreviewConverter<
    PricePreviewTemplate<ProductData, PriceData>,
    PricePreviewTemplate<ProductData, PriceData>> {
    static auto DoConvert(const PricePreviewTemplate<ProductData, PriceData>& price_preview
    ) -> PricePreviewTemplate<ProductData, PriceData> {
        return price_preview;
    }
};

// Serializing converter
template <typename FromProductData, typename FromPriceData, typename ToProductData, typename ToPriceData>
struct PricePreviewConverter<
    PricePreviewTemplate<FromProductData, FromPriceData>,
    PricePreviewTemplate<ToProductData, ToPriceData>> {
    static auto DoConvert(const PricePreviewTemplate<FromProductData, FromPriceData>& price_preview
    ) -> PricePreviewTemplate<ToProductData, ToPriceData> {
        using ItemType = ItemTemplate<ToProductData, ToPriceData>;
        PricePreviewTemplate<ToProductData, ToPriceData> result;
        result.customer_id = price_preview.customer_id;
        result.address_id = price_preview.address_id;
        result.business_id = price_preview.business_id;
        result.currency_code = price_preview.currency_code;
        result.discount_id = price_preview.discount_id;
        result.address = price_preview.address;
        result.customer_ip_address = price_preview.customer_ip_address;
        result.details.line_items.reserve(price_preview.details.line_items.size());
        for (const auto& item : price_preview.details.line_items) {
            result.details.line_items.push_back(Convert(item, convert::To<ItemType>{}));
        }
        result.available_payment_methods = price_preview.available_payment_methods;
        return result;
    }
};

}  // namespace impl

template <typename FromProductData, typename FromPriceData, typename ToProductData, typename ToPriceData>
auto Convert(const PricePreviewTemplate<FromProductData, FromPriceData>& price_preview, convert::To<PricePreviewTemplate<ToProductData, ToPriceData>>)
    -> PricePreviewTemplate<ToProductData, ToPriceData> {
    return impl::PricePreviewConverter<
        PricePreviewTemplate<FromProductData, FromPriceData>,
        PricePreviewTemplate<ToProductData, ToPriceData>>::DoConvert(price_preview);
}

}  // namespace paddle::prices
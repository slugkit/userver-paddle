#pragma once

#include <paddle/types/convert.hpp>
#include <paddle/types/enums.hpp>
#include <paddle/types/ids.hpp>
#include <paddle/types/response.hpp>
#include <paddle/types/timestamp.hpp>

namespace paddle::products {

template <typename CustomData>
struct ProductTemplate {
    using IdType = ProductId;

    ProductId id;
    std::string name;
    std::optional<std::string> description;
    CatalogType type;
    std::optional<std::string> tax_category;

    std::optional<std::string> image_url;

    JSON custom_data;
    Status status;

    // there is some import meta, but we don't care about it

    Timestamp created_at;
    Timestamp updated_at;
};

using JsonProduct = ProductTemplate<JSON>;

template <typename CustomData = JSON>
using ProductsResponse = Response<ProductTemplate<CustomData>, MetaPaginated>;

}  // namespace paddle::products

namespace paddle::products {

template <typename CustomData, typename Format>
auto Serialize(const ProductTemplate<CustomData>& product, userver::formats::serialize::To<Format>) -> Format {
    typename Format::Builder builder;
    builder["id"] = product.id;
    builder["name"] = product.name;
    builder["description"] = product.description;
    builder["type"] = product.type;
    builder["tax_category"] = product.tax_category;
    builder["image_url"] = product.image_url;
    builder["custom_data"] = product.custom_data;
    builder["status"] = product.status;
    builder["created_at"] = product.created_at;
    builder["updated_at"] = product.updated_at;
    return builder.ExtractValue();
}

template <typename Value, typename CustomData>
auto Parse(const Value& value, userver::formats::parse::To<ProductTemplate<CustomData>>)
    -> ProductTemplate<CustomData> {
    ProductTemplate<CustomData> product;
    product.id = value["id"].template As<ProductId>();
    product.name = value["name"].template As<std::string>();
    product.description = value["description"].template As<std::optional<std::string>>();
    product.type = value["type"].template As<CatalogType>();
    product.tax_category = value["tax_category"].template As<std::optional<std::string>>();
    product.image_url = value["image_url"].template As<std::optional<std::string>>();
    product.custom_data = value["custom_data"];
    product.status = value["status"].template As<Status>();
    product.created_at = value["created_at"].template As<Timestamp>();
    product.updated_at = value["updated_at"].template As<Timestamp>();
    return product;
}

namespace impl {

template <typename From, typename To>
struct ProductConverter;

// Identity converter
template <typename CustomData>
struct ProductConverter<CustomData, CustomData> {
    static auto Convert(const ProductTemplate<CustomData>& product) -> ProductTemplate<CustomData> {
        return product;
    }
};

// Serializing converter
template <typename From, FormatType To>
requires SerializableTo<From, To>
struct ProductConverter<From, To> {
    static auto Convert(const ProductTemplate<From>& product) -> ProductTemplate<To> {
        ProductTemplate<To> result;
        result.id = product.id;
        result.name = product.name;
        result.description = product.description;
        result.type = product.type;
        result.tax_category = product.tax_category;
        result.image_url = product.image_url;
        result.custom_data = Serialize(product.custom_data, userver::formats::serialize::To<To>{});
        result.status = product.status;
        result.created_at = product.created_at;
        result.updated_at = product.updated_at;
        return result;
    }
};

// Parsing converter
template <FormatType From, typename To>
requires ParseableFrom<To, From>
struct ProductConverter<From, To> {
    static auto Convert(const ProductTemplate<From>& product) -> ProductTemplate<To> {
        ProductTemplate<To> result;
        result.id = product.id;
        result.name = product.name;
        result.description = product.description;
        result.type = product.type;
        result.tax_category = product.tax_category;
        result.image_url = product.image_url;
        result.custom_data = Parse(product.custom_data, userver::formats::parse::To<To>{});
        result.status = product.status;
        result.created_at = product.created_at;
        result.updated_at = product.updated_at;
        return result;
    }
};
}  // namespace impl

template <typename From, FormatType Format>
requires SerializableTo<From, Format>
auto Convert(const ProductTemplate<From>& product, convert::To<ProductTemplate<Format>>) -> ProductTemplate<Format> {
    return impl::ProductConverter<From, Format>::Convert(product);
}

template <FormatType Format, typename To>
requires ParseableFrom<To, Format>
auto Convert(const ProductTemplate<Format>& product, convert::To<ProductTemplate<To>>) -> ProductTemplate<To> {
    return impl::ProductConverter<Format, To>::Convert(product);
}

template <typename T>
auto Convert(const ProductTemplate<T>& product, convert::To<ProductTemplate<T>>) -> ProductTemplate<T> {
    return impl::ProductConverter<T, T>::Convert(product);
}

template <typename To, typename From>
auto Convert(const ProductTemplate<From>& product) -> ProductTemplate<To> {
    return Convert(product, convert::To<ProductTemplate<To>>{});
}

}  // namespace paddle::products
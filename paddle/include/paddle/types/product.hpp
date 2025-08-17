#pragma once

#include <paddle/types/enums.hpp>
#include <paddle/types/ids.hpp>
#include <paddle/types/response.hpp>
#include <paddle/types/timestamp.hpp>

namespace paddle::products {

struct Product {
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

using ProductsResponse = Response<Product, MetaPaginated>;

}  // namespace paddle::products

namespace paddle::products {

template <typename Format>
Format Serialize(const Product& product, userver::formats::serialize::To<Format>) {
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

template <typename Value>
Product Parse(const Value& value, userver::formats::parse::To<Product>) {
    Product product;
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

}  // namespace paddle::products
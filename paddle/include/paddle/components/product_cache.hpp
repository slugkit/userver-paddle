#pragma once

#include <paddle/components/client.hpp>
#include <paddle/types/formats.hpp>
#include <paddle/types/fwd.hpp>
#include <paddle/types/ids.hpp>
#include <paddle/types/product.hpp>

#include <userver/cache/caching_component_base.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/http/http_request.hpp>
#include <userver/utils/fast_pimpl.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include <string>
#include <unordered_map>

namespace paddle::components {

template <typename CustomData>
struct ProductCacheTraits {
    using KeyType = ProductId;
    using ProductType = products::ProductTemplate<CustomData>;
    using CustomDataType = CustomData;

    static auto GetId(const ProductType& product) -> KeyType {
        return product.custom_data;
    }
};

template <>
struct ProductCacheTraits<JSON> {
    using KeyType = ProductId;
    using ProductType = products::JsonProduct;
    using CustomDataType = JSON;

    static auto GetId(const ProductType& product) -> KeyType {
        return product.id;
    }
};

template <typename CustomData>
struct IdProductTraits {
    using KeyType = ProductId;
    using ProductType = products::ProductTemplate<CustomData>;
    using CustomDataType = CustomData;

    static auto GetId(const ProductType& product) -> KeyType {
        return product.id;
    }
};

namespace impl {
/// @brief Base class for product cache
/// @note This class IS NOT in usever's components hierarchy,
///       it is only used to provide a base class for the product cache
/// This class fetches JSON products and calls overriden methods to add or update products
class ProductCacheBase {
public:
    using JsonProductType = products::JsonProduct;
    using JsonProductList = std::vector<JsonProductType>;
    using ProductListCallback = std::function<void(JsonProductList&& products)>;

    ProductCacheBase(const Client& client, std::int32_t per_page = 200)
        : client_(client)
        , per_page_(per_page) {
    }

    virtual ~ProductCacheBase() = default;

    virtual auto AddProduct(const JsonProductType& product) -> void = 0;
    virtual auto UpdateProduct(const JsonProductType& product) -> void = 0;

protected:
    /// @brief Fetches JSON products page by page
    auto FetchProducts(userver::cache::UpdateStatisticsScope& stats_scope, ProductListCallback callback) -> void;

private:
    const Client& client_;
    std::int32_t per_page_;
};

}  // namespace impl

template <typename CustomData, typename PayloadTraits = ProductCacheTraits<CustomData>>
class ProductCache final
    : public userver::components::CachingComponentBase<
          std::unordered_map<typename PayloadTraits::KeyType, typename PayloadTraits::ProductType>>,
      public impl::ProductCacheBase {
public:
    using JsonProductType = impl::ProductCacheBase::JsonProductType;
    using ProductType = typename PayloadTraits::ProductType;
    using CustomDataType = typename PayloadTraits::CustomDataType;
    using ComponentBaseType =
        userver::components::CachingComponentBase<std::unordered_map<typename PayloadTraits::KeyType, ProductType>>;
    using BaseType = impl::ProductCacheBase;
    using TraitsType = PayloadTraits;
    using DataType = typename ComponentBaseType::DataType;

public:
    ProductCache(
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    );
    ~ProductCache() override;

    static auto GetStaticConfigSchema() -> userver::yaml_config::Schema;

    auto AddProduct(const JsonProductType& product) -> void;
    auto UpdateProduct(const JsonProductType& product) -> void;

    template <typename T>
    auto AddProduct(const products::ProductTemplate<T>& product) -> void
    requires(!std::is_same_v<T, JSON>);

    template <typename T>
    auto UpdateProduct(const products::ProductTemplate<T>& product) -> void
    requires(!std::is_same_v<T, JSON>);

private:
    template <typename T>
    auto DoAddProduct(const products::ProductTemplate<T>& product) -> void;
    template <typename T>
    auto DoUpdateProduct(const products::ProductTemplate<T>& product) -> void;

private:
    auto Update(
        userver::cache::UpdateType type,
        const std::chrono::system_clock::time_point& last_update,
        const std::chrono::system_clock::time_point& now,
        userver::cache::UpdateStatisticsScope& stats_scope
    ) -> void override;
};

//----------------------------------------------------
// Implementation
//----------------------------------------------------

template <typename CustomData, typename PayloadTraits>
ProductCache<CustomData, PayloadTraits>::ProductCache(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
)
    : ComponentBaseType(config, context)
    , BaseType(context.FindComponent<Client>(config["client_name"].As<std::string>("paddle-client"))) {
    this->StartPeriodicUpdates();
}

template <typename CustomData, typename PayloadTraits>
ProductCache<CustomData, PayloadTraits>::~ProductCache() {
    this->StopPeriodicUpdates();
}

template <typename CustomData, typename PayloadTraits>
auto ProductCache<CustomData, PayloadTraits>::GetStaticConfigSchema() -> userver::yaml_config::Schema {
    return userver::yaml_config::MergeSchemas<ComponentBaseType>(R"(
type: object
description: Product cache component
additionalProperties: false
properties:
    client_name:
        type: string
        description: Component name for Paddle client (paddle-client by default)
    )");
}

template <typename CustomData, typename PayloadTraits>
auto ProductCache<CustomData, PayloadTraits>::AddProduct(const JsonProductType& product) -> void {
    this->DoAddProduct(product);
}

template <typename CustomData, typename PayloadTraits>
auto ProductCache<CustomData, PayloadTraits>::UpdateProduct(const JsonProductType& product) -> void {
    this->DoUpdateProduct(product);
}

template <typename CustomData, typename PayloadTraits>
template <typename T>
auto ProductCache<CustomData, PayloadTraits>::AddProduct(const products::ProductTemplate<T>& product) -> void
requires(!std::is_same_v<T, JSON>)
{
    this->DoAddProduct(product);
}

template <typename CustomData, typename PayloadTraits>
template <typename T>
auto ProductCache<CustomData, PayloadTraits>::UpdateProduct(const products::ProductTemplate<T>& product) -> void
requires(!std::is_same_v<T, JSON>)
{
    this->DoUpdateProduct(product);
}

template <typename CustomData, typename PayloadTraits>
template <typename T>
auto ProductCache<CustomData, PayloadTraits>::DoAddProduct(const products::ProductTemplate<T>& product) -> void {
    auto existing_data = this->Get();
    auto new_data = std::make_unique<DataType>(*existing_data);
    try {
        auto converted_product = Convert<CustomData>(product);
        auto id = TraitsType::GetId(converted_product);
        new_data->insert_or_assign(std::move(id), std::move(converted_product));
    } catch (const std::exception& e) {
        LOG_ERROR() << "Error converting product " << product.id << ": " << e.what();
    }
    this->Set(std::move(new_data));
}

template <typename CustomData, typename PayloadTraits>
template <typename T>
auto ProductCache<CustomData, PayloadTraits>::DoUpdateProduct(const products::ProductTemplate<T>& product) -> void {
    auto existing_data = this->Get();
    auto new_data = std::make_unique<DataType>(*existing_data);
    try {
        auto converted_product = Convert<CustomData>(product);
        auto id = TraitsType::GetId(converted_product);
        new_data->insert_or_assign(std::move(id), std::move(converted_product));
    } catch (const std::exception& e) {
        LOG_ERROR() << "Error converting product " << product.id << ": " << e.what();
    }
    this->Set(std::move(new_data));
}

template <typename CustomData, typename PayloadTraits>
auto ProductCache<CustomData, PayloadTraits>::Update(
    [[maybe_unused]] userver::cache::UpdateType type,
    [[maybe_unused]] const std::chrono::system_clock::time_point& last_update,
    [[maybe_unused]] const std::chrono::system_clock::time_point& now,
    userver::cache::UpdateStatisticsScope& stats_scope
) -> void {
    auto data = std::make_unique<DataType>();
    this->FetchProducts(stats_scope, [&data](JsonProductList&& products) {
        for (auto&& product : products) {
            try {
                auto converted_product = Convert<CustomData>(product);
                auto id = TraitsType::GetId(converted_product);
                data->insert_or_assign(std::move(id), std::move(converted_product));
            } catch (const std::exception& e) {
                LOG_ERROR() << "Error converting product " << product.id << ": " << e.what();
            }
        }
    });
    stats_scope.Finish(data->size());
    this->Set(std::move(data));
}

}  // namespace paddle::components
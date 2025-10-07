#pragma once

#include <paddle/components/client.hpp>
#include <paddle/types/ids.hpp>
#include <paddle/types/price.hpp>

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

template <typename PricePayload>
struct PriceCacheTraits {
    using KeyType = PricePayload;
    using PriceType = prices::PriceTemplate<PricePayload>;
    using CustomDataType = PricePayload;

    static auto GetId(const PriceType& price) -> KeyType {
        return price.custom_data;
    }
};

template <>
struct PriceCacheTraits<JSON> {
    using KeyType = PriceId;
    using PriceType = prices::JsonPrice;
    using CustomDataType = JSON;

    static auto GetId(const PriceType& price) -> KeyType {
        return price.id;
    }
};

template <typename PricePayload>
struct IdPriceTraits {
    using KeyType = PriceId;
    using PriceType = prices::PriceTemplate<PricePayload>;
    using CustomDataType = PricePayload;

    static auto GetId(const PriceType& price) -> KeyType {
        return price.id;
    }
};

namespace impl {
/// @brief Base class for price cache
/// @note This class IS NOT in usever's components hierarchy,
///       it is only used to provide a base class for the price cache
/// This class fetches JSON prices and calls overriden methods to add or update prices
class PriceCacheBase {
public:
    using JsonPriceType = prices::JsonPrice;
    using JsonPriceList = std::vector<JsonPriceType>;
    using PriceListCallback = std::function<void(JsonPriceList&& prices)>;

    PriceCacheBase(const Client& client, std::int32_t per_page = 200)
        : client_{client}
        , per_page_{per_page} {
    }
    virtual ~PriceCacheBase() = default;

    virtual auto AddPrice(const JsonPriceType& price) -> void = 0;
    virtual auto UpdatePrice(const JsonPriceType& price) -> void = 0;
    virtual auto RemovePrice(const JsonPriceType& price) -> void = 0;

protected:
    /// @brief Fetches JSON prices page by page
    auto FetchPrices(userver::cache::UpdateStatisticsScope& stats_scope, PriceListCallback callback) -> void;

private:
    const Client& client_;
    std::int32_t per_page_;
};

}  // namespace impl

template <typename PricePayload, typename PayloadTraits = PriceCacheTraits<PricePayload>>
class PriceCache final : public userver::components::CachingComponentBase<
                             std::unordered_map<typename PayloadTraits::KeyType, typename PayloadTraits::PriceType>>,
                         public impl::PriceCacheBase {
public:
    using JsonPriceType = impl::PriceCacheBase::JsonPriceType;
    using PriceType = typename PayloadTraits::PriceType;
    using CustomDataType = typename PayloadTraits::CustomDataType;
    using ComponentBaseType =
        userver::components::CachingComponentBase<std::unordered_map<typename PayloadTraits::KeyType, PriceType>>;
    using BaseType = impl::PriceCacheBase;
    using TraitsType = PayloadTraits;
    using DataType = typename ComponentBaseType::DataType;

public:
    PriceCache(
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    );
    ~PriceCache() override;

    static auto GetStaticConfigSchema() -> userver::yaml_config::Schema;

    auto AddPrice(const JsonPriceType& price) -> void override;
    auto UpdatePrice(const JsonPriceType& price) -> void override;
    auto RemovePrice(const JsonPriceType& price) -> void override;

    template <typename T>
    auto AddPrice(const prices::PriceTemplate<T>& price) -> void
    requires(!std::is_same_v<T, JSON>);

    template <typename T>
    auto UpdatePrice(const prices::PriceTemplate<T>& price) -> void
    requires(!std::is_same_v<T, JSON>);

    template <typename T>
    auto RemovePrice(const prices::PriceTemplate<T>& price) -> void
    requires(!std::is_same_v<T, JSON>);

private:
    template <typename T>
    auto DoAddPrice(const prices::PriceTemplate<T>& price) -> void;
    template <typename T>
    auto DoUpdatePrice(const prices::PriceTemplate<T>& price) -> void;
    template <typename T>
    auto DoRemovePrice(const prices::PriceTemplate<T>& price) -> void;

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
template <typename PricePayload, typename PayloadTraits>
PriceCache<PricePayload, PayloadTraits>::PriceCache(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
)
    : ComponentBaseType(config, context)
    , BaseType(context.FindComponent<Client>(config["client_name"].As<std::string>("paddle-client"))) {
    this->StartPeriodicUpdates();
}

template <typename PricePayload, typename PayloadTraits>
PriceCache<PricePayload, PayloadTraits>::~PriceCache() {
    this->StopPeriodicUpdates();
}

template <typename PricePayload, typename PayloadTraits>
auto PriceCache<PricePayload, PayloadTraits>::GetStaticConfigSchema() -> userver::yaml_config::Schema {
    return userver::yaml_config::MergeSchemas<ComponentBaseType>(R"(
type: object
description: Price cache component
additionalProperties: false
properties:
    client_name:
        type: string
        description: Component name for Paddle client (paddle-client by default)
    )");
}

template <typename PricePayload, typename PayloadTraits>
auto PriceCache<PricePayload, PayloadTraits>::AddPrice(const JsonPriceType& price) -> void {
    this->DoAddPrice(price);
}

template <typename PricePayload, typename PayloadTraits>
auto PriceCache<PricePayload, PayloadTraits>::UpdatePrice(const JsonPriceType& price) -> void {
    this->DoUpdatePrice(price);
}

template <typename PricePayload, typename PayloadTraits>
auto PriceCache<PricePayload, PayloadTraits>::RemovePrice(const JsonPriceType& price) -> void {
    this->DoRemovePrice(price);
}

template <typename PricePayload, typename PayloadTraits>
template <typename T>
auto PriceCache<PricePayload, PayloadTraits>::AddPrice(const prices::PriceTemplate<T>& price) -> void
requires(!std::is_same_v<T, JSON>)
{
    this->DoAddPrice(price);
}

template <typename PricePayload, typename PayloadTraits>
template <typename T>
auto PriceCache<PricePayload, PayloadTraits>::UpdatePrice(const prices::PriceTemplate<T>& price) -> void
requires(!std::is_same_v<T, JSON>)
{
    this->DoUpdatePrice(price);
}

template <typename PricePayload, typename PayloadTraits>
template <typename T>
auto PriceCache<PricePayload, PayloadTraits>::RemovePrice(const prices::PriceTemplate<T>& price) -> void
requires(!std::is_same_v<T, JSON>)
{
    this->DoRemovePrice(price);
}

template <typename PricePayload, typename PayloadTraits>
template <typename T>
auto PriceCache<PricePayload, PayloadTraits>::DoAddPrice(const prices::PriceTemplate<T>& price) -> void {
    auto existing_data = this->Get();
    auto new_data = std::make_unique<DataType>(*existing_data);
    try {
        auto converted_price = Convert<CustomDataType>(price);
        auto id = TraitsType::GetId(converted_price);
        new_data->insert_or_assign(std::move(id), std::move(converted_price));
    } catch (const std::exception& e) {
        LOG_ERROR() << "Error converting price " << price.id << ": " << e.what();
    }
    this->Set(std::move(new_data));
}

template <typename PricePayload, typename PayloadTraits>
template <typename T>
auto PriceCache<PricePayload, PayloadTraits>::DoUpdatePrice(const prices::PriceTemplate<T>& price) -> void {
    auto existing_data = this->Get();
    auto new_data = std::make_unique<DataType>(*existing_data);
    try {
        auto converted_price = Convert<CustomDataType>(price);
        auto id = TraitsType::GetId(converted_price);
        new_data->insert_or_assign(std::move(id), std::move(converted_price));
    } catch (const std::exception& e) {
        LOG_ERROR() << "Error converting price " << price.id << ": " << e.what();
    }
    this->Set(std::move(new_data));
}

template <typename PricePayload, typename PayloadTraits>
template <typename T>
auto PriceCache<PricePayload, PayloadTraits>::DoRemovePrice(const prices::PriceTemplate<T>& price) -> void {
    auto existing_data = this->Get();
    auto new_data = std::make_unique<DataType>(*existing_data);
    try {
        auto converted_price = Convert<CustomDataType>(price);
        auto id = TraitsType::GetId(converted_price);
        new_data->erase(id);
    } catch (const std::exception& e) {
        LOG_ERROR() << "Error converting price " << price.id << ": " << e.what();
    }
    this->Set(std::move(new_data));
}

template <typename PricePayload, typename PayloadTraits>
auto PriceCache<PricePayload, PayloadTraits>::Update(
    [[maybe_unused]] userver::cache::UpdateType type,
    [[maybe_unused]] const std::chrono::system_clock::time_point& last_update,
    [[maybe_unused]] const std::chrono::system_clock::time_point& now,
    userver::cache::UpdateStatisticsScope& stats_scope
) -> void {
    auto data_cache = std::make_unique<DataType>();
    this->FetchPrices(stats_scope, [&data_cache](JsonPriceList&& prices) {
        for (auto&& price : prices) {
            try {
                auto converted_price = Convert<CustomDataType>(price);
                auto id = TraitsType::GetId(converted_price);
                data_cache->insert_or_assign(std::move(id), std::move(converted_price));
            } catch (const std::exception& e) {
                LOG_ERROR() << "Error converting price " << price.id << ": " << e.what();
            }
        }
    });
    stats_scope.Finish(data_cache->size());
    this->Set(std::move(data_cache));
}

}  // namespace paddle::components
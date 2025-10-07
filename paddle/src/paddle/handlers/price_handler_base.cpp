#include <paddle/handlers/price_handler_base.hpp>

#include <paddle/types/events.hpp>
#include <paddle/types/price.hpp>

#include <paddle/components/price_cache.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/logging/log.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include <fmt/format.h>

namespace paddle::handlers {

struct PriceHandlerBase::Impl {
    std::vector<std::string> price_cache_names;
    std::vector<components::impl::PriceCacheBase*> price_caches;

    Impl(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
        : price_cache_names(config["update-caches"].As<std::vector<std::string>>()) {
        for (auto& price_cache_name : price_cache_names) {
            price_caches.push_back(context.FindComponentOptional<components::impl::PriceCacheBase>(price_cache_name));
        }
    }

    auto AddPrice(const PriceType& price) const -> void {
        for (auto& price_cache : price_caches) {
            if (price_cache) {
                price_cache->AddPrice(price);
            }
        }
    }

    auto UpdatePrice(const PriceType& price) const -> void {
        for (auto& price_cache : price_caches) {
            if (price_cache) {
                price_cache->UpdatePrice(price);
            }
        }
    }
};

PriceHandlerBase::PriceHandlerBase(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
)
    : BaseType{config, context}
    , impl_{config, context} {
}

PriceHandlerBase::~PriceHandlerBase() = default;

auto PriceHandlerBase::GetStaticConfigSchema() -> userver::yaml_config::Schema {
    return userver::yaml_config::MergeSchemas<BaseType>(R"(
type: object
description: Price handler base component
additionalProperties: false
properties:
    update-caches:
        type: array
        items:
            type: string
        description: |
            Names of the price cache components to update
    )");
}

auto PriceHandlerBase::HandleEvent([[maybe_unused]] const JSON& request_json, EventType&& event) const -> void {
    LOG_INFO() << "Handling event: " << event.event_type << " " << event.event_id;
    switch (event.event_type) {
        case events::EventTypeName::kPriceCreated:
            HandleCreated(std::move(event));
            break;
        case events::EventTypeName::kPriceImported:
            HandleImported(std::move(event));
            break;
        case events::EventTypeName::kPriceUpdated:
            HandleUpdated(std::move(event));
            break;
        default:
            LOG_INFO() << "Event handling not implemented for event type: " << event.event_type;
    }
}

auto PriceHandlerBase::HandleCreated(EventType&& event) const -> void {
    impl_->AddPrice(event.data);
    DoHandleCreated(std::move(event));
}

auto PriceHandlerBase::DoHandleCreated(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto PriceHandlerBase::HandleImported(EventType&& event) const -> void {
    impl_->AddPrice(event.data);
    DoHandleImported(std::move(event));
}

auto PriceHandlerBase::DoHandleImported(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto PriceHandlerBase::HandleUpdated(EventType&& event) const -> void {
    impl_->UpdatePrice(event.data);
    DoHandleUpdated(std::move(event));
}

auto PriceHandlerBase::DoHandleUpdated(EventType&& event) const -> void {
    LogEventIgnored(event);
}

}  // namespace paddle::handlers

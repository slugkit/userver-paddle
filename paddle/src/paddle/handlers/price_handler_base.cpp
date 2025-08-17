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
    components::PriceCache* price_cache;

    Impl(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
        : price_cache(
              context.FindComponentOptional<components::PriceCache>(config["price_cache_name"].As<std::string>(components::PriceCache::kName))
          ) {
    }

    void InvalidateCache() const {
        if (price_cache) {
            price_cache->InvalidateAsync(userver::cache::UpdateType::kFull);
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

    return userver::yaml_config::MergeSchemas<BaseType>(R"(
type: object
description: Price handler base component
additionalProperties: false
properties:
    price_cache_name:
        type: string
        description: Name of the price cache component
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
    DoHandleCreated(std::move(event));
    impl_->InvalidateCache();
}

auto PriceHandlerBase::DoHandleCreated(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto PriceHandlerBase::HandleImported(EventType&& event) const -> void {
    DoHandleImported(std::move(event));
    impl_->InvalidateCache();
}

auto PriceHandlerBase::DoHandleImported(EventType&& event) const -> void {
    LogEventIgnored(event);
}

auto PriceHandlerBase::HandleUpdated(EventType&& event) const -> void {
    DoHandleUpdated(std::move(event));
    impl_->InvalidateCache();
}

auto PriceHandlerBase::DoHandleUpdated(EventType&& event) const -> void {
    LogEventIgnored(event);
}

}  // namespace paddle::handlers

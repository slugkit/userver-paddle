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
  components::PriceCache *price_cache;

  Impl(const userver::components::ComponentConfig &config,
       const userver::components::ComponentContext &context)
      : price_cache(context.FindComponentOptional<components::PriceCache>(
            config["price_cache_name"].As<std::string>(
                components::PriceCache::kName))) {}

  auto AddPrice(const prices::Price &price) const -> void {
    if (price_cache) {
      price_cache->AddPrice(price);
    }
  }

  auto UpdatePrice(const prices::Price &price) const -> void {
    if (price_cache) {
      price_cache->UpdatePrice(price);
    }
  }
};

PriceHandlerBase::PriceHandlerBase(
    const userver::components::ComponentConfig &config,
    const userver::components::ComponentContext &context)
    : BaseType{config, context}, impl_{config, context} {}

PriceHandlerBase::~PriceHandlerBase() = default;

auto PriceHandlerBase::GetStaticConfigSchema() -> userver::yaml_config::Schema {
  return userver::yaml_config::MergeSchemas<BaseType>(R"(
type: object
description: Price handler base component
additionalProperties: false
properties:
    price_cache_name:
        type: string
        description: |
            Name of the price cache component (default: paddle-price-cache)
    )");
}

auto PriceHandlerBase::HandleEvent([[maybe_unused]] const JSON &request_json,
                                   EventType &&event) const -> void {
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
    LOG_INFO() << "Event handling not implemented for event type: "
               << event.event_type;
  }
}

auto PriceHandlerBase::HandleCreated(EventType &&event) const -> void {
  impl_->AddPrice(event.data);
  DoHandleCreated(std::move(event));
}

auto PriceHandlerBase::DoHandleCreated(EventType &&event) const -> void {
  LogEventIgnored(event);
}

auto PriceHandlerBase::HandleImported(EventType &&event) const -> void {
  impl_->AddPrice(event.data);
  DoHandleImported(std::move(event));
}

auto PriceHandlerBase::DoHandleImported(EventType &&event) const -> void {
  LogEventIgnored(event);
}

auto PriceHandlerBase::HandleUpdated(EventType &&event) const -> void {
  impl_->UpdatePrice(event.data);
  DoHandleUpdated(std::move(event));
}

auto PriceHandlerBase::DoHandleUpdated(EventType &&event) const -> void {
  LogEventIgnored(event);
}

} // namespace paddle::handlers

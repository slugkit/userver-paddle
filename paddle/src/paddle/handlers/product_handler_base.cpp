#include <paddle/handlers/product_handler_base.hpp>

#include <paddle/types/product.hpp>

#include <paddle/components/product_cache.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/logging/log.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include <fmt/format.h>

namespace paddle::handlers {

struct ProductHandlerBase::Impl {
  components::ProductCache *product_cache;

  Impl(const userver::components::ComponentConfig &config,
       const userver::components::ComponentContext &context)
      : product_cache(context.FindComponentOptional<components::ProductCache>(
            config["product_cache_name"].As<std::string>(
                components::ProductCache::kName))) {}

  auto AddProduct(const products::Product &product) const -> void {
    if (product_cache) {
      product_cache->AddProduct(product);
    }
  }

  auto UpdateProduct(const products::Product &product) const -> void {
    if (product_cache) {
      product_cache->UpdateProduct(product);
    }
  }
};

ProductHandlerBase::ProductHandlerBase(
    const userver::components::ComponentConfig &config,
    const userver::components::ComponentContext &context)
    : BaseType{config, context}, impl_{config, context} {}

ProductHandlerBase::~ProductHandlerBase() = default;

auto ProductHandlerBase::GetStaticConfigSchema()
    -> userver::yaml_config::Schema {
  return userver::yaml_config::MergeSchemas<BaseType>(R"(
type: object
description: Product handler base component
additionalProperties: false
properties:
    product_cache_name:
        type: string
        description: |
            Name of the product cache component (default: paddle-product-cache)
    )");
}

auto ProductHandlerBase::HandleEvent([[maybe_unused]] const JSON &request_json,
                                     EventType &&event) const -> void {
  LOG_INFO() << "Handling event: " << event.event_type << " " << event.event_id;
  switch (event.event_type) {
  case events::EventTypeName::kProductCreated:
    HandleCreated(std::move(event));
    break;
  case events::EventTypeName::kProductImported:
    HandleImported(std::move(event));
    break;
  case events::EventTypeName::kProductUpdated:
    HandleUpdated(std::move(event));
    break;
  default:
    LOG_INFO() << "Event handling not implemented for event type: "
               << event.event_type;
  }
}

auto ProductHandlerBase::HandleCreated(EventType &&event) const -> void {
  impl_->AddProduct(event.data);
  DoHandleCreated(std::move(event));
}

auto ProductHandlerBase::DoHandleCreated(EventType &&event) const -> void {
  LogEventIgnored(event);
}

auto ProductHandlerBase::HandleImported(EventType &&event) const -> void {
  impl_->AddProduct(event.data);
  DoHandleImported(std::move(event));
}

auto ProductHandlerBase::DoHandleImported(EventType &&event) const -> void {
  LogEventIgnored(event);
}

auto ProductHandlerBase::HandleUpdated(EventType &&event) const -> void {
  impl_->UpdateProduct(event.data);
  DoHandleUpdated(std::move(event));
}

auto ProductHandlerBase::DoHandleUpdated(EventType &&event) const -> void {
  LogEventIgnored(event);
}

} // namespace paddle::handlers
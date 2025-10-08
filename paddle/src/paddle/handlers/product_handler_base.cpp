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
    std::vector<std::string> product_cache_names;
    std::vector<components::impl::ProductCacheBase*> product_caches;

    Impl(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
        : product_cache_names(config["update-caches"].As<std::vector<std::string>>()) {
        for (auto& product_cache_name : product_cache_names) {
            product_caches.push_back(
                context.FindComponentOptional<components::impl::ProductCacheBase>(product_cache_name)
            );
        }
    }

    auto AddProduct(const products::JsonProduct& product) const -> void {
        for (auto& product_cache : product_caches) {
            if (product_cache) {
                product_cache->AddProduct(product);
            }
        }
    }

    auto UpdateProduct(const products::JsonProduct& product) const -> void {
        for (auto& product_cache : product_caches) {
            if (product_cache) {
                product_cache->UpdateProduct(product);
            }
        }
    }
};

ProductHandlerBase::ProductHandlerBase(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
)
    : BaseType{config, context}
    , impl_{config, context} {
}

ProductHandlerBase::~ProductHandlerBase() = default;

auto ProductHandlerBase::GetStaticConfigSchema() -> userver::yaml_config::Schema {
    return userver::yaml_config::MergeSchemas<BaseType>(R"(
type: object
description: Product handler base component
additionalProperties: false
properties:
    update-caches:
        type: array
        description: |
            Names of the product cache components (default: paddle-product-cache)
        items:
          type: string
          description: |
              Name of the product cache component
    )");
}

auto ProductHandlerBase::HandleEvent([[maybe_unused]] const JSON& request_json, EventType&& event) const
    -> HandleResult {
    LOG_INFO() << "Handling event: " << event.event_type << " " << event.event_id;
    switch (event.event_type) {
        case events::EventTypeName::kProductCreated:
            return HandleCreated(std::move(event));
        case events::EventTypeName::kProductImported:
            return HandleImported(std::move(event));
        case events::EventTypeName::kProductUpdated:
            return HandleUpdated(std::move(event));
        default:
            LOG_INFO() << "Event handling not implemented for event type: " << event.event_type;
    }
    return HandleResult{
        HandleResultStatus::kIgnored, "Event handling not implemented for event type: " + EnumToString(event.event_type)
    };
}

auto ProductHandlerBase::HandleCreated(EventType&& event) const -> HandleResult {
    impl_->AddProduct(event.data);
    return DoHandleCreated(std::move(event));
}

auto ProductHandlerBase::DoHandleCreated(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto ProductHandlerBase::HandleImported(EventType&& event) const -> HandleResult {
    impl_->AddProduct(event.data);
    return DoHandleImported(std::move(event));
}

auto ProductHandlerBase::DoHandleImported(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

auto ProductHandlerBase::HandleUpdated(EventType&& event) const -> HandleResult {
    impl_->UpdateProduct(event.data);
    return DoHandleUpdated(std::move(event));
}

auto ProductHandlerBase::DoHandleUpdated(EventType&& event) const -> HandleResult {
    return LogEventIgnored(event);
}

}  // namespace paddle::handlers

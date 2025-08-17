#pragma once

#include <paddle/types/events.hpp>
#include <paddle/types/formats.hpp>
#include <paddle/types/fwd.hpp>

#include <userver/components/component_base.hpp>
#include <userver/utils/fast_pimpl.hpp>

namespace paddle::handlers {

class ProductHandlerBase : public userver::components::ComponentBase {
public:
  using BaseType = userver::components::ComponentBase;
  using EventType = events::Event<products::Product>;
  constexpr static auto kEventCategory = events::EventCategory::kProduct;

  ProductHandlerBase(const userver::components::ComponentConfig &config,
                     const userver::components::ComponentContext &context);
  ~ProductHandlerBase() override;

  static auto GetStaticConfigSchema() -> userver::yaml_config::Schema;

  auto HandleEvent(const JSON &request_json, EventType &&event) const -> void;

  auto HandleCreated(EventType &&event) const -> void;
  auto HandleImported(EventType &&event) const -> void;
  auto HandleUpdated(EventType &&event) const -> void;

private:
  virtual auto DoHandleCreated(EventType &&) const -> void;
  virtual auto DoHandleImported(EventType &&) const -> void;
  virtual auto DoHandleUpdated(EventType &&) const -> void;

private:
  constexpr static auto kImplSize = 8UL;
  constexpr static auto kImplAlign = 8UL;
  struct Impl;
  userver::utils::FastPimpl<Impl, kImplSize, kImplAlign> impl_;
};

} // namespace paddle::handlers
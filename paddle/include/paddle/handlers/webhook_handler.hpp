#pragma once

#include <paddle/types/fwd.hpp>

#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utils/fast_pimpl.hpp>

namespace paddle::handlers {

class WebhookHandler final : public userver::server::handlers::HttpHandlerJsonBase {
public:
    using BaseType = userver::server::handlers::HttpHandlerJsonBase;

    WebhookHandler(
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    );
    ~WebhookHandler() override;

    static auto GetStaticConfigSchema() -> userver::yaml_config::Schema;

    auto HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value& request_json,
        userver::server::request::RequestContext& context
    ) const -> userver::formats::json::Value final;

private:
    constexpr static auto kImplSize = 272UL;
    constexpr static auto kImplAlign = 16UL;
    struct Impl;
    userver::utils::FastPimpl<Impl, kImplSize, kImplAlign> impl_;
};

}  // namespace paddle::handlers
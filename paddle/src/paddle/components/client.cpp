#include <paddle/components/client.hpp>

#include <paddle/types/price.hpp>
#include <paddle/types/product.hpp>
#include <paddle/types/subscriptions.hpp>

#include <userver/clients/http/client.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/logging/log.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

namespace paddle::components {

struct Client::Impl {
    userver::components::HttpClient& http_client;
    std::string api_version;
    std::string base_url;
    std::string api_key;

    Impl(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
        : http_client(context.FindComponent<userver::components::HttpClient>())
        , api_version(config["api-version"].As<std::string>("1"))
        , base_url(config["base-url"].As<std::string>("https://api.paddle.com"))
        , api_key("Bearer " + config["api-key"].As<std::string>()) {
    }

    void ThrowIfNotOk(
        std::shared_ptr<userver::clients::http::Response> response,
        std::string_view request_path,
        std::string_view operation
    ) const {
        if (response->status_code() != 200) {
            LOG_ERROR() << fmt::format("Failed to {} {}: {}", operation, request_path, response->status_code());
            throw std::runtime_error(
                fmt::format("Failed to {} {}: {}", operation, request_path, response->status_code())
            );
        }
    }

    template <typename T>
    ResponseWithCursor<T> GetPaginated(std::string_view path, std::string_view cursor, std::int32_t per_page) const {
        auto url = fmt::format("{}/{}?per_page={}&order_by=id[ASC]", base_url, path, per_page);
        if (!cursor.empty()) {
            url += fmt::format("&after={}", cursor);
        }
        auto http_response = http_client.GetHttpClient()
                                 .CreateRequest()
                                 .get(url)
                                 .headers({
                                     {"Authorization", api_key},
                                     {"Paddle-Api-Version", api_version},
                                 })
                                 .timeout(std::chrono::seconds(30))
                                 .perform();
        ThrowIfNotOk(http_response, url, "get paginated");
        auto body = http_response->body();
        auto json = userver::formats::json::FromString(body);
        auto response = json.As<Response<T, MetaPaginated>>();
        // Now we need to find the next cursor
        // The cursor is in meta.pagination.next
        // We need to extract the cursor from the next field, which is a url
        // that has ?after=<cursor>
        auto pos = response.meta.pagination.next.find("after=");
        if (pos == std::string::npos) {
            return {std::move(response.data), {}, false};
        }
        auto next_cursor = response.meta.pagination.next.substr(pos + 6);
        return {std::move(response.data), next_cursor, response.meta.pagination.has_more};
    }

    template <typename Result, typename Request>
    Result Post(std::string_view path, std::string_view operation, const Request& request) const {
        auto request_body = Serialize(request, userver::formats::serialize::To<JSON>{});
        auto request_path = fmt::format("{}/{}", base_url, path);
        auto response = http_client.GetHttpClient()
                            .CreateRequest()
                            .post(request_path, ToString(request_body))
                            .headers({
                                {"Authorization", api_key},
                                {"Paddle-Api-Version", api_version},
                                {"Content-Type", "application/json"},
                            })
                            .timeout(std::chrono::seconds(30))
                            .perform();
        ThrowIfNotOk(response, request_path, operation);
        auto body = response->body();
        try {
            auto result = userver::formats::json::FromString(body).template As<Result>();
            return result;
        } catch (const std::exception& e) {
            LOG_ERROR() << fmt::format("Failed to parse response for {}: {}\n{}", operation, e.what(), body);
            throw;
        }
    }

    template <typename T>
    std::vector<T> GetAll(std::string_view path, std::int32_t per_page) const {
        std::vector<T> data;
        std::string cursor;
        bool has_more = true;
        while (has_more) {
            auto response = GetPaginated<T>(path, cursor, per_page);
            data.insert(data.end(), std::get<0>(response).begin(), std::get<0>(response).end());
            cursor = std::get<1>(response);
            has_more = std::get<2>(response);
        }
        return data;
    }

    std::vector<NotificationSetting> GetAllNotificationSettings() const {
        return GetAll<NotificationSetting>("notification-settings", 200);
    }

    ResponseWithCursor<NotificationSetting> GetNotificationSettings(std::string_view cursor, std::int32_t per_page)
        const {
        return GetPaginated<NotificationSetting>("notification-settings", cursor, per_page);
    }

    std::vector<events::Event<JSON>> GetAllEvents() const {
        return GetAll<events::Event<JSON>>("events", 200);
    }

    ResponseWithCursor<events::Event<JSON>> GetEvents(std::string_view cursor, std::int32_t per_page) const {
        return GetPaginated<events::Event<JSON>>("events", cursor, per_page);
    }

    ResponseWithCursor<products::JsonProduct> GetProducts(std::string_view cursor, std::int32_t per_page) const {
        return GetPaginated<products::JsonProduct>("products", cursor, per_page);
    }

    std::vector<products::JsonProduct> GetAllProducts() const {
        LOG_INFO() << "Getting all products";
        return GetAll<products::JsonProduct>("products", 200);
    }

    ResponseWithCursor<prices::JsonPrice> GetPrices(std::string_view cursor, std::int32_t per_page) const {
        return GetPaginated<prices::JsonPrice>("prices", cursor, per_page);
    }

    std::vector<prices::JsonPrice> GetAllPrices() const {
        LOG_INFO() << "Getting all prices";
        return GetAll<prices::JsonPrice>("prices", 200);
    }

    prices::JsonPricePreview GetPricePreview(const prices::PricePreviewRequest& request) const {
        return Post<SingleObjectResponse<prices::JsonPricePreview, Meta>>(
                   "pricing-preview", "get price preview", request
        )
            .data;
    }
};

Client::Client(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
    : userver::components::ComponentBase(config, context)
    , impl_{config, context} {
}

Client::~Client() = default;

userver::yaml_config::Schema Client::GetStaticConfigSchema() {
    return userver::yaml_config::MergeSchemas<userver::components::ComponentBase>(
        R"(
type: object
description: Paddle client component
additionalProperties: false
properties:
    base-url:
        type: string
        description: base URL of the Paddle API
    api-key:
        type: string
        description: API key for the Paddle API
    api-version:
        type: string
        description: API version to use
    )"
    );
}

std::vector<NotificationSetting> Client::GetAllNotificationSettings() const {
    return impl_->GetAllNotificationSettings();
}

ResponseWithCursor<NotificationSetting> Client::GetNotificationSettings(std::string_view cursor, std::int32_t per_page)
    const {
    return impl_->GetNotificationSettings(cursor, per_page);
}

std::vector<events::Event<JSON>> Client::GetAllEvents() const {
    return impl_->GetAllEvents();
}

ResponseWithCursor<events::Event<JSON>> Client::GetEvents(std::string_view cursor, std::int32_t per_page) const {
    return impl_->GetEvents(cursor, per_page);
}

std::vector<products::JsonProduct> Client::GetAllProducts() const {
    return impl_->GetAllProducts();
}

ResponseWithCursor<products::JsonProduct> Client::GetProducts(std::string_view cursor, std::int32_t per_page) const {
    return impl_->GetProducts(cursor, per_page);
}

std::vector<prices::JsonPrice> Client::GetAllPrices() const {
    return impl_->GetAllPrices();
}

ResponseWithCursor<prices::JsonPrice> Client::GetPrices(std::string_view cursor, std::int32_t per_page) const {
    return impl_->GetPrices(cursor, per_page);
}

prices::JsonPricePreview Client::GetPricePreview(const prices::PricePreviewRequest& request) const {
    return impl_->GetPricePreview(request);
}

}  // namespace paddle::components

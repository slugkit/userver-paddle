#pragma once

#include <paddle/types/ids.hpp>
#include <paddle/types/permissions.hpp>
#include <paddle/types/timestamp.hpp>

namespace paddle::api_keys {

enum class Status {
    kActive,
    kExpired,
    kRevoked,
};

struct ApiKeyEventPayload {
    ApiKeyId id;
    std::string name;
    std::optional<std::string> description;
    std::string key;  ///< An obfuscated version of this API key, prefixed with `pdl_` and containing `_apikey_ `.
    Status status;
    std::vector<permissions::Permission> permissions;
    std::optional<Timestamp> expires_at;
    std::optional<Timestamp> last_used_at;
    Timestamp created_at;
    Timestamp updated_at;
};

}  // namespace paddle::api_keys

template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::api_keys::Status>
    : EnumMappingBase<paddle::api_keys::Status> {
    static constexpr DBTypeName postgres_name = "paddle.api_key_status";
    static constexpr userver::utils::TrivialBiMap enumerators = [](auto selector) {
        return selector()
            .Case("active", EnumType::kActive)
            .Case("expired", EnumType::kExpired)
            .Case("revoked", EnumType::kRevoked);
    };
};

namespace paddle::api_keys {

// Status
template <typename Format>
Format Serialize(const Status& status, userver::formats::serialize::To<Format> to) {
    return SerializeEnum(status, to);
}

template <typename Value>
Status Parse(const Value& value, userver::formats::parse::To<Status> to) {
    return ParseEnum(value, to);
}

// ApiKeyEventPayload
template <typename Format>
Format Serialize(const ApiKeyEventPayload& payload, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["id"] = payload.id;
    builder["name"] = payload.name;
    builder["description"] = payload.description;
    builder["key"] = payload.key;
    builder["status"] = payload.status;
    builder["permissions"] = payload.permissions;
    builder["expires_at"] = payload.expires_at;
    builder["last_used_at"] = payload.last_used_at;
    builder["created_at"] = payload.created_at;
    builder["updated_at"] = payload.updated_at;
    return builder.ExtractValue();
}

template <typename Value>
ApiKeyEventPayload Parse(const Value& value, userver::formats::parse::To<ApiKeyEventPayload>) {
    ApiKeyEventPayload payload;
    payload.id = value["id"].template As<ApiKeyId>();
    payload.name = value["name"].template As<std::string>();
    payload.description = value["description"].template As<std::optional<std::string>>();
    payload.key = value["key"].template As<std::string>();
    payload.status = value["status"].template As<Status>();
    payload.permissions = value["permissions"].template As<std::vector<permissions::Permission>>();
    payload.expires_at = value["expires_at"].template As<std::optional<Timestamp>>();
    payload.last_used_at = value["last_used_at"].template As<std::optional<Timestamp>>();
    payload.created_at = value["created_at"].template As<Timestamp>();
    payload.updated_at = value["updated_at"].template As<Timestamp>();
    return payload;
}

}  // namespace paddle::api_keys
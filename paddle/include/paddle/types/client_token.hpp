#pragma once

#include <paddle/types/enums.hpp>
#include <paddle/types/ids.hpp>
#include <paddle/types/timestamp.hpp>

#include <paddle/types/response.hpp>

namespace paddle::client_tokens {

struct ClientToken {
    ClientTokenId id;
    std::string token;
    std::string name;
    std::optional<std::string> description;
    ClientTokenStatus status;
    OptionalTimestamp revoked_at;
    Timestamp created_at;
    Timestamp updated_at;
};

}  // namespace paddle::client_tokens

namespace paddle::client_tokens {

// ClientToken
template <typename Format>
Format Serialize(const ClientToken& token, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["id"] = token.id;
    builder["token"] = token.token;
    builder["name"] = token.name;
    builder["status"] = token.status;
    builder["revoked_at"] = token.revoked_at;
    builder["created_at"] = token.created_at;
    builder["updated_at"] = token.updated_at;
    return builder.ExtractValue();
}

template <typename Value>
ClientToken Parse(const Value& value, userver::formats::parse::To<ClientToken>) {
    ClientToken token;
    token.id = value["id"].template As<ClientTokenId>();
    token.token = value["token"].template As<std::string>();
    token.name = value["name"].template As<std::string>();
    token.status = value["status"].template As<ClientTokenStatus>();
    token.revoked_at = value["revoked_at"].template As<OptionalTimestamp>();
    token.created_at = value["created_at"].template As<Timestamp>();
    token.updated_at = value["updated_at"].template As<Timestamp>();
    return token;
}

}  // namespace paddle::client_tokens

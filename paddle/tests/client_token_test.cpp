#include <paddle/types/client_token.hpp>

#include <userver/utest/utest.hpp>

namespace paddle {

namespace {
const auto kClientTokenJson = R"({
    "id": "ctkn_01k2jhg6f70pv7hk6n5k82px9g",
    "token": "test_1234567890",
    "name": "Sandbox Token",
    "description": null,
    "status": "active",
    "revoked_at": null,
    "created_at": "2025-08-13T20:21:16.90399Z",
    "updated_at": "2025-08-13T20:21:16.903991Z"
})";
}

UTEST(ClientTokenTest, Parse) {
    auto json = userver::formats::json::FromString(kClientTokenJson);
    auto token = Parse(json, userver::formats::parse::To<client_tokens::ClientToken>());
    EXPECT_EQ(token.id, ClientTokenId("ctkn_01k2jhg6f70pv7hk6n5k82px9g"));
    EXPECT_EQ(token.token, "test_1234567890");
    EXPECT_EQ(token.name, "Sandbox Token");
    EXPECT_EQ(token.status, ClientTokenStatus::kActive);
    EXPECT_EQ(token.revoked_at, std::nullopt);
}

}  // namespace paddle

#include <paddle/types/money.hpp>

#include <userver/utest/utest.hpp>

namespace paddle {

namespace {
const auto kMoneyJson = R"({"amount":"100","currency_code":"USD"})";

const auto kMoney = money::Money{100, money::CurrencyCode{"USD"}};
}  // namespace

UTEST(Money, Serialize) {
    auto expected = userver::formats::json::FromString(kMoneyJson);
    auto actual = Serialize(kMoney, userver::formats::serialize::To<userver::formats::json::Value>());
    EXPECT_EQ(actual, expected);
}

UTEST(Money, Parse) {
    auto json = userver::formats::json::FromString(kMoneyJson);
    auto money = Parse(json, userver::formats::parse::To<money::Money>());
    EXPECT_EQ(money, kMoney);
}

}  // namespace paddle

#pragma once

#include <paddle/types/duration.hpp>
#include <paddle/types/ids.hpp>

namespace paddle {

struct BillingDetails {
    bool enable_checkout;
    std::string purchase_order_number;
    std::optional<std::string> additional_information;
    Duration payment_terms;
};

using OptionalBillingDetails = std::optional<BillingDetails>;

}  // namespace paddle

namespace paddle {

template <typename Format>
Format Serialize(const BillingDetails& billing_details, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["enable_checkout"] = billing_details.enable_checkout;
    builder["purchase_order_number"] = billing_details.purchase_order_number;
    if (billing_details.additional_information) {
        builder["additional_information"] = *billing_details.additional_information;
    }
    builder["payment_terms"] = billing_details.payment_terms;
    return builder.ExtractValue();
}

template <typename Value>
BillingDetails Parse(const Value& value, userver::formats::parse::To<BillingDetails>) {
    BillingDetails billing_details;
    if (value.HasMember("enable_checkout")) {
        billing_details.enable_checkout = value["enable_checkout"].template As<bool>();
    } else {
        billing_details.enable_checkout = false;
    }
    if (value.HasMember("purchase_order_number")) {
        billing_details.purchase_order_number = value["purchase_order_number"].template As<std::string>();
    }
    if (value.HasMember("additional_information")) {
        billing_details.additional_information =
            value["additional_information"].template As<std::optional<std::string>>();
    }
    billing_details.payment_terms = value["payment_terms"].template As<Duration>();
    return billing_details;
}

}  // namespace paddle
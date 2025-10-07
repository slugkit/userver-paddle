#pragma once

#include <paddle/types/billing.hpp>
#include <paddle/types/enums.hpp>
#include <paddle/types/ids.hpp>
#include <paddle/types/money.hpp>
#include <paddle/types/payment_method.hpp>
#include <paddle/types/price.hpp>
#include <paddle/types/product.hpp>
#include <paddle/types/timestamp.hpp>

namespace paddle::transactions {

/// How proration was calculated for this item. Populated when a transaction is
/// created from a subscription change, where `proration_billing_mode` was
/// `prorated_immediately` or `prorated_next_billing_period`. Set automatically
/// by Paddle.
struct ItemProration {
    std::string rate;
    TimePeriod billing_period;
};

struct Item {
    OptionalPriceId price_id;
    std::optional<prices::JsonPrice> price;
    std::int32_t quantity;
    std::optional<ItemProration> proration;
};

/// Breakdown of a charge in the lowest denomination of a currency (e.g. cents
/// for USD). JSON contains strings, but we use integers for calculations.
struct Totals {
    /// Subtotal before discount, tax, and deductions. If an item, unit price
    /// multiplied by quantity.
    std::int64_t subtotal;
    /// Total discount as a result of any discounts applied.
    //
    // Except for percentage discounts, Paddle applies tax to discounts based on
    // the line item `price.tax_mode`. If `price.tax_mode` for a line item is
    // `internal`, Paddle removes tax from the discount applied.
    std::int64_t discount;
    /// Total tax on the subtotal.
    std::int64_t tax;
    /// Total amount after discount and tax.
    std::int64_t total;
};

/// Information about line items for this transaction. Different from
/// transaction `items` as they include totals calculated by Paddle. Considered
/// the source of truth for line item totals.
struct LineItem {
    OptionalTransactionItemId id;
    PriceId price_id;
    /// Quantity of this transaction line item.
    std::int32_t quantity;
    /// How proration was calculated for this item. Populated when a transaction
    /// is created from a subscription change, where `proration_billing_mode` was
    /// `prorated_immediately` or `prorated_next_billing_period`. Set
    /// automatically by Paddle.
    std::optional<ItemProration> proration;
    /// Rate used to calculate tax for this transaction line item.
    std::string tax_rate;
    /// Breakdown of the charge for one unit in the lowest denomination of a
    /// currency (e.g. cents for USD).
    Totals unit_totals;
    /// Breakdown of the charge for this transaction line item.
    Totals totals;
    /// Related product entity for this transaction line item price. Reflects the
    /// entity at the time it was added to the transaction.
    products::JsonProduct product;
};

/// List of tax rates applied for this transaction.
struct TaxRateUsed {
    std::string tax_rate;
    Totals totals;
};

/// Breakdown of the total for a transaction. These numbers can be negative when
/// dealing with subscription updates that result in credit. Numbers come in as
/// strings.
struct TransactionTotals {
    /// Total credit applied to this transaction. This includes credits applied
    /// using a customer's credit balance and adjustments to a `billed`
    /// transaction.
    std::int64_t credit;
    /// Additional credit generated from negative `details.line_items`. This
    /// credit is added to the customer balance.
    std::int64_t credit_to_balance;
    /// Total due on a transaction after credits and any payments.
    std::int64_t balance;
    /// Total due on a transaction after credits but before any payments.
    std::int64_t grand_total;
    /// Total fee taken by Paddle for this transaction. `null` until the
    /// transaction is `completed` and the fee is processed.
    std::optional<std::int64_t> fee;
    /// Total earnings for this transaction. This is the total minus the Paddle
    /// fee. `null` until the transaction is `completed` and the fee is processed.
    std::optional<std::int64_t> earnings;
    /// Three-letter ISO 4217 currency code of the currency used for this
    /// transaction.
    money::CurrencyCode currency_code;
};

/// Breakdown of the totals for a transaction after adjustments.
struct TransactionTotalsAdjusted {
    /// Subtotal before discount, tax, and deductions. If an item, unit price
    /// multiplied by quantity.
    std::int64_t subtotal;
    /// Total tax on the subtotal.
    std::int64_t tax;
    /// Total amount after tax.
    std::int64_t total;
    /// Total due after credits but before any payments.
    std::int64_t grand_total;
    /// Total fee taken by Paddle for this transaction. `null` until the
    /// transaction is `completed` and the fee is processed.
    std::optional<std::int64_t> fee;
    /// Total earnings for this transaction. This is the total minus the Paddle
    /// fee. `null` until the transaction is `completed` and the fee is processed.
    std::optional<std::int64_t> earnings;
    /// Three-letter ISO 4217 currency code of the currency used for this
    /// transaction.
    money::CurrencyCode currency_code;
};

/// Breakdown of the payout total for a transaction. `null` until the
/// transaction is `completed`. Returned in your payout currency.
struct TransactionPayoutTotals {
    /// Total before tax and fees.
    std::int64_t subtotal;
    /// Total discount as a result of any discounts applied.
    /// Except for percentage discounts, Paddle applies tax to discounts based on
    /// the line item `price.tax_mode`. If `price.tax_mode` for a line item is
    /// `internal`, Paddle removes tax from the discount applied.
    std::int64_t discount;
    /// Total tax on the subtotal.
    std::int64_t tax;
    /// Total amount after tax.
    std::int64_t total;
    /// Total credit applied to this transaction. This includes credits applied
    /// using a customer's credit balance and adjustments to a `billed`
    /// transaction.
    std::int64_t credit;
    /// Additional credit generated from negative `details.line_items`. This
    /// credit is added to the customer balance.
    std::int64_t credit_to_balance;
    /// Total due on a transaction after credits and any payments.
    std::int64_t balance;
    /// Total due on a transaction after credits but before any payments.
    std::int64_t grand_total;
    /// Total fee taken by Paddle for this payout.
    std::optional<std::int64_t> fee;
    /// Total earnings for this payout. This is the subtotal minus the Paddle fee.
    std::optional<std::int64_t> earnings;
    /// Three-letter ISO 4217 currency code used for the payout for this
    /// transaction. If your primary currency has changed, this reflects the
    /// primary currency at the time the transaction was billed.
    money::CurrencyCode currency_code;
};

/// Details of any chargeback fees incurred for this transaction.
struct ChargebackFee {
    /// Chargeback fee converted into the payout currency.
    std::int64_t amount;
    /// Chargeback fee before conversion to the payout currency. `null` when the
    /// chargeback fee is the same as the payout currency.
    std::optional<money::Money> original;
};

/// Breakdown of the payout total for a transaction after adjustments. `null`
/// until the transaction is `completed`.
struct TransactionPayoutTotalsAdjusted {
    /// Total before tax and fees.
    std::int64_t subtotal;
    /// Total tax on the subtotal.
    std::int64_t tax;
    /// Total after tax.
    std::int64_t total;
    /// Total fee taken by Paddle for this payout.
    std::int64_t fee;
    /// Details of any chargeback fees incurred for this transaction.
    ChargebackFee chargeback_fee;
    /// Total earnings for this payout. This is the subtotal minus the Paddle fee,
    /// excluding chargeback fees.
    std::int64_t earnings;
    /// Three-letter ISO 4217 currency code used for the payout for this
    /// transaction. If your primary currency has changed, this reflects the
    /// primary currency at the time the transaction was billed.
    money::CurrencyCode currency_code;
};

/// Calculated totals for a transaction, including proration, discounts, tax,
/// and currency conversion. Considered the source of truth for totals on a
/// transaction.
struct Details {
    std::vector<TaxRateUsed> tax_rates_used;
    TransactionTotals totals;
    std::optional<TransactionTotalsAdjusted> adjusted_totals;
    /// Breakdown of the payout total for a transaction. `null` until the
    /// transaction is `completed`. Returned in your payout currency.
    std::optional<TransactionPayoutTotals> payout_totals;
    /// Breakdown of the payout total for a transaction after adjustments. `null`
    /// until the transaction is `completed`.
    std::optional<TransactionPayoutTotalsAdjusted> payout_totals_adjusted;
    /// Information about line items for this transaction. Different from
    /// transaction `items` as they include totals calculated by Paddle.
    /// Considered the source of truth for line item totals.
    std::vector<LineItem> line_items;
};

struct PaymentMethod {
    PaymentMethodType type;
    /// Details of the payment method used for this payment attempt.
    /// There is some structured stuff, but we don't use it.
    JSON underlying_details;
    std::optional<money::Card> card;
};

struct PaymentAttempt {
    PaymentAttemptId payment_attempt_id;
    /// UUID for the stored payment method used for this payment attempt.
    /// Deprecated - use `payment_method_id` instead. but the field is still
    /// present for backwards compatibility.
    boost::uuids::uuid stored_payment_method_id;
    /// Paddle ID of the payment method used for this payment attempt.
    OptionalPaymentMethodId payment_method_id;
    /// Amount for collection in the lowest denomination of a currency (e.g. cents
    /// for USD). string in json.
    std::int64_t amount;
    /// Status of the payment attempt.
    PaymentAttemptStatus status;
    /// Error code for the payment attempt.
    std::optional<PaymentAttemptErrorCode> error_code;
    PaymentMethod method_details;
    /// Timestamp of the payment attempt.
    Timestamp created_at;
    /// Timestamp of the payment attempt.
    OptionalTimestamp captured_at;
};

struct Checkout {
    std::optional<std::string> url;
};

struct Transaction {
    TransactionId id;
    TransactionStatus status;
    OptionalCustomerId customer_id;
    OptionalAddressId address_id;
    OptionalBusinessId business_id;
    JSON custom_data;
    money::CurrencyCode currency_code;
    TransactionOrigin origin;
    OptionalSubscriptionId subscription_id;
    std::optional<std::string> invoice_id;  // may be dropped
    OptionalDocumentNumber document_number;
    CollectionMode collection_mode;
    OptionalDiscountId discount_id;
    std::optional<BillingDetails> billing_details;
    std::optional<TimePeriod> billing_period;
    std::vector<Item> items;
    Details details;
    std::vector<PaymentAttempt> payments;
    std::optional<Checkout> checkout;
    Timestamp created_at;
    Timestamp updated_at;
    OptionalTimestamp billed_at;
    OptionalTimestamp revised_at;
};

}  // namespace paddle::transactions

namespace paddle::transactions {

// TransactionItemProration
template <typename Format>
Format Serialize(const ItemProration& proration, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["rate"] = proration.rate;
    builder["billing_period"] = proration.billing_period;
    return builder.ExtractValue();
}

template <typename Value>
ItemProration Parse(const Value& value, userver::formats::parse::To<ItemProration>) {
    ItemProration proration;
    proration.rate = value["rate"].template As<std::string>();
    proration.billing_period = value["billing_period"].template As<TimePeriod>();
    return proration;
}

// Item
template <typename Format>
Format Serialize(const Item& item, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["price_id"] = item.price_id;
    builder["price"] = item.price;
    builder["quantity"] = item.quantity;
    builder["proration"] = item.proration;
    return builder.ExtractValue();
}

template <typename Value>
Item Parse(const Value& value, userver::formats::parse::To<Item>) {
    Item item;
    if (value.HasMember("price_id")) {
        item.price_id = value["price_id"].template As<OptionalPriceId>();
    }
    if (value.HasMember("price")) {
        item.price = value["price"].template As<std::optional<prices::JsonPrice>>();
    }
    item.quantity = value["quantity"].template As<std::int32_t>();
    if (value.HasMember("proration")) {
        item.proration = value["proration"].template As<std::optional<ItemProration>>();
    }
    return item;
}

// Totals
template <typename Format>
Format Serialize(const Totals& totals, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["subtotal"] = std::to_string(totals.subtotal);
    builder["discount"] = std::to_string(totals.discount);
    builder["tax"] = std::to_string(totals.tax);
    builder["total"] = std::to_string(totals.total);
    return builder.ExtractValue();
}

template <typename Value>
Totals Parse(const Value& value, userver::formats::parse::To<Totals>) {
    Totals totals;
    totals.subtotal = std::stoll(value["subtotal"].template As<std::string>());
    totals.discount = std::stoll(value["discount"].template As<std::string>());
    totals.tax = std::stoll(value["tax"].template As<std::string>());
    totals.total = std::stoll(value["total"].template As<std::string>());
    return totals;
}

// LineItem
template <typename Format>
Format Serialize(const LineItem& line_item, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["id"] = line_item.id;
    builder["price_id"] = line_item.price_id;
    builder["quantity"] = line_item.quantity;
    builder["proration"] = line_item.proration;
    builder["tax_rate"] = line_item.tax_rate;
    builder["unit_totals"] = line_item.unit_totals;
    builder["totals"] = line_item.totals;
    builder["product"] = line_item.product;
    return builder.ExtractValue();
}

template <typename Value>
LineItem Parse(const Value& value, userver::formats::parse::To<LineItem>) {
    LineItem line_item;
    line_item.id = value["id"].template As<OptionalTransactionItemId>();
    line_item.price_id = value["price_id"].template As<PriceId>();
    line_item.quantity = value["quantity"].template As<std::int32_t>();
    if (value.HasMember("proration")) {
        line_item.proration = value["proration"].template As<std::optional<ItemProration>>();
    }
    line_item.tax_rate = value["tax_rate"].template As<std::string>();
    line_item.unit_totals = value["unit_totals"].template As<Totals>();
    line_item.totals = value["totals"].template As<Totals>();
    line_item.product = value["product"].template As<products::JsonProduct>();
    return line_item;
}

// TaxRateUsed
template <typename Format>
Format Serialize(const TaxRateUsed& tax_rate_used, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["tax_rate"] = tax_rate_used.tax_rate;
    builder["totals"] = tax_rate_used.totals;
    return builder.ExtractValue();
}

template <typename Value>
TaxRateUsed Parse(const Value& value, userver::formats::parse::To<TaxRateUsed>) {
    TaxRateUsed tax_rate_used;
    tax_rate_used.tax_rate = value["tax_rate"].template As<std::string>();
    tax_rate_used.totals = value["totals"].template As<Totals>();
    return tax_rate_used;
}

// TransactionTotals
template <typename Format>
Format Serialize(const TransactionTotals& totals, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["credit"] = std::to_string(totals.credit);
    builder["credit_to_balance"] = std::to_string(totals.credit_to_balance);
    builder["balance"] = std::to_string(totals.balance);
    builder["grand_total"] = std::to_string(totals.grand_total);
    if (totals.fee) {
        builder["fee"] = std::to_string(*totals.fee);
    } else {
        builder["fee"] = std::nullopt;
    }
    if (totals.earnings) {
        builder["earnings"] = std::to_string(*totals.earnings);
    } else {
        builder["earnings"] = std::nullopt;
    }
    builder["currency_code"] = totals.currency_code;
    return builder.ExtractValue();
}

template <typename Value>
TransactionTotals Parse(const Value& value, userver::formats::parse::To<TransactionTotals>) {
    TransactionTotals totals;
    totals.credit = std::stoll(value["credit"].template As<std::string>());
    totals.credit_to_balance = std::stoll(value["credit_to_balance"].template As<std::string>());
    totals.balance = std::stoll(value["balance"].template As<std::string>());
    totals.grand_total = std::stoll(value["grand_total"].template As<std::string>());
    if (value.HasMember("fee") && !value["fee"].IsNull()) {
        totals.fee = std::stoll(value["fee"].template As<std::string>());
    }
    if (value.HasMember("earnings") && !value["earnings"].IsNull()) {
        totals.earnings = std::stoll(value["earnings"].template As<std::string>());
    }
    totals.currency_code = value["currency_code"].template As<money::CurrencyCode>();
    return totals;
}

// TransactionTotalsAdjusted
template <typename Format>
Format Serialize(const TransactionTotalsAdjusted& totals, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["subtotal"] = std::to_string(totals.subtotal);
    builder["tax"] = std::to_string(totals.tax);
    builder["total"] = std::to_string(totals.total);
    builder["grand_total"] = std::to_string(totals.grand_total);
    if (totals.fee) {
        builder["fee"] = std::to_string(*totals.fee);
    } else {
        builder["fee"] = std::nullopt;
    }
    if (totals.earnings) {
        builder["earnings"] = std::to_string(*totals.earnings);
    } else {
        builder["earnings"] = std::nullopt;
    }
    builder["currency_code"] = totals.currency_code;
    return builder.ExtractValue();
}

template <typename Value>
TransactionTotalsAdjusted Parse(const Value& value, userver::formats::parse::To<TransactionTotalsAdjusted>) {
    TransactionTotalsAdjusted totals;
    totals.subtotal = std::stoll(value["subtotal"].template As<std::string>());
    totals.tax = std::stoll(value["tax"].template As<std::string>());
    totals.total = std::stoll(value["total"].template As<std::string>());
    totals.grand_total = std::stoll(value["grand_total"].template As<std::string>());
    if (value.HasMember("fee") && !value["fee"].IsNull()) {
        totals.fee = std::stoll(value["fee"].template As<std::string>());
    }
    if (value.HasMember("earnings") && !value["earnings"].IsNull()) {
        totals.earnings = std::stoll(value["earnings"].template As<std::string>());
    }
    totals.currency_code = value["currency_code"].template As<money::CurrencyCode>();
    return totals;
}

// TransactionPayoutTotals
template <typename Format>
Format Serialize(const TransactionPayoutTotals& totals, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["subtotal"] = std::to_string(totals.subtotal);
    builder["discount"] = std::to_string(totals.discount);
    builder["tax"] = std::to_string(totals.tax);
    builder["total"] = std::to_string(totals.total);
    builder["credit"] = std::to_string(totals.credit);
    builder["credit_to_balance"] = std::to_string(totals.credit_to_balance);
    builder["balance"] = std::to_string(totals.balance);
    builder["grand_total"] = std::to_string(totals.grand_total);
    if (totals.fee) {
        builder["fee"] = std::to_string(*totals.fee);
    } else {
        builder["fee"] = std::nullopt;
    }
    if (totals.earnings) {
        builder["earnings"] = std::to_string(*totals.earnings);
    } else {
        builder["earnings"] = std::nullopt;
    }
    builder["currency_code"] = totals.currency_code;
    return builder.ExtractValue();
}

template <typename Value>
TransactionPayoutTotals Parse(const Value& value, userver::formats::parse::To<TransactionPayoutTotals>) {
    TransactionPayoutTotals totals;
    totals.subtotal = std::stoll(value["subtotal"].template As<std::string>());
    totals.discount = std::stoll(value["discount"].template As<std::string>());
    totals.tax = std::stoll(value["tax"].template As<std::string>());
    totals.total = std::stoll(value["total"].template As<std::string>());
    totals.credit = std::stoll(value["credit"].template As<std::string>());
    totals.credit_to_balance = std::stoll(value["credit_to_balance"].template As<std::string>());
    totals.balance = std::stoll(value["balance"].template As<std::string>());
    totals.grand_total = std::stoll(value["grand_total"].template As<std::string>());
    if (value.HasMember("fee") && !value["fee"].IsNull()) {
        totals.fee = std::stoll(value["fee"].template As<std::string>());
    }
    if (value.HasMember("earnings") && !value["earnings"].IsNull()) {
        totals.earnings = std::stoll(value["earnings"].template As<std::string>());
    }
    totals.currency_code = value["currency_code"].template As<money::CurrencyCode>();
    return totals;
}

// ChargebackFee
template <typename Format>
Format Serialize(const ChargebackFee& chargeback_fee, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["amount"] = std::to_string(chargeback_fee.amount);
    builder["original"] = chargeback_fee.original;
    return builder.ExtractValue();
}

template <typename Value>
ChargebackFee Parse(const Value& value, userver::formats::parse::To<ChargebackFee>) {
    ChargebackFee chargeback_fee;
    chargeback_fee.amount = std::stoll(value["amount"].template As<std::string>());
    if (value.HasMember("original") && !value["original"].IsNull()) {
        chargeback_fee.original = value["original"].template As<money::Money>();
    }
    return chargeback_fee;
}

// TransactionPayoutTotalsAdjusted
template <typename Format>
Format Serialize(const TransactionPayoutTotalsAdjusted& totals, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["subtotal"] = std::to_string(totals.subtotal);
    builder["tax"] = std::to_string(totals.tax);
    builder["total"] = std::to_string(totals.total);
    builder["fee"] = std::to_string(totals.fee);
    builder["chargeback_fee"] = totals.chargeback_fee;
    builder["earnings"] = std::to_string(totals.earnings);
    builder["currency_code"] = totals.currency_code;
    return builder.ExtractValue();
}

template <typename Value>
TransactionPayoutTotalsAdjusted
Parse(const Value& value, userver::formats::parse::To<TransactionPayoutTotalsAdjusted>) {
    TransactionPayoutTotalsAdjusted totals;
    totals.subtotal = std::stoll(value["subtotal"].template As<std::string>());
    totals.tax = std::stoll(value["tax"].template As<std::string>());
    totals.total = std::stoll(value["total"].template As<std::string>());
    totals.fee = std::stoll(value["fee"].template As<std::string>());
    totals.chargeback_fee = value["chargeback_fee"].template As<ChargebackFee>();
    totals.earnings = std::stoll(value["earnings"].template As<std::string>());
    totals.currency_code = value["currency_code"].template As<money::CurrencyCode>();
    return totals;
}

// Details
template <typename Format>
Format Serialize(const Details& details, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["tax_rates_used"] = details.tax_rates_used;
    builder["totals"] = details.totals;
    builder["adjusted_totals"] = details.adjusted_totals;
    builder["payout_totals"] = details.payout_totals;
    builder["payout_totals_adjusted"] = details.payout_totals_adjusted;
    builder["line_items"] = details.line_items;
    return builder.ExtractValue();
}

template <typename Value>
Details Parse(const Value& value, userver::formats::parse::To<Details>) {
    Details details;
    details.tax_rates_used = value["tax_rates_used"].template As<std::vector<TaxRateUsed>>();
    details.totals = value["totals"].template As<TransactionTotals>();
    if (value.HasMember("adjusted_totals") && !value["adjusted_totals"].IsNull()) {
        details.adjusted_totals = value["adjusted_totals"].template As<TransactionTotalsAdjusted>();
    }
    if (value.HasMember("payout_totals") && !value["payout_totals"].IsNull()) {
        details.payout_totals = value["payout_totals"].template As<TransactionPayoutTotals>();
    }
    if (value.HasMember("payout_totals_adjusted") && !value["payout_totals_adjusted"].IsNull()) {
        details.payout_totals_adjusted = value["payout_totals_adjusted"].template As<TransactionPayoutTotalsAdjusted>();
    }
    details.line_items = value["line_items"].template As<std::vector<LineItem>>();
    return details;
}

// PaymentMethod
template <typename Format>
Format Serialize(const PaymentMethod& payment_method, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["type"] = payment_method.type;
    builder["underlying_details"] = payment_method.underlying_details;
    builder["card"] = payment_method.card;
    return builder.ExtractValue();
}

template <typename Value>
PaymentMethod Parse(const Value& value, userver::formats::parse::To<PaymentMethod>) {
    PaymentMethod payment_method;
    payment_method.type = value["type"].template As<PaymentMethodType>();
    payment_method.underlying_details = value["underlying_details"].template As<JSON>();
    payment_method.card = value["card"].template As<money::Card>();
    return payment_method;
}

// PaymentAttempt
template <typename Format>
Format Serialize(const PaymentAttempt& payment_attempt, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["payment_attempt_id"] = payment_attempt.payment_attempt_id;
    builder["stored_payment_method_id"] = payment_attempt.stored_payment_method_id;
    builder["payment_method_id"] = payment_attempt.payment_method_id;
    builder["amount"] = std::to_string(payment_attempt.amount);
    builder["status"] = payment_attempt.status;
    builder["error_code"] = payment_attempt.error_code;
    builder["method_details"] = payment_attempt.method_details;
    builder["created_at"] = payment_attempt.created_at;
    builder["captured_at"] = payment_attempt.captured_at;
    return builder.ExtractValue();
}

template <typename Value>
PaymentAttempt Parse(const Value& value, userver::formats::parse::To<PaymentAttempt>) {
    PaymentAttempt payment_attempt;
    payment_attempt.payment_attempt_id = value["payment_attempt_id"].template As<PaymentAttemptId>();
    if (value.HasMember("stored_payment_method_id")) {
        payment_attempt.stored_payment_method_id = value["stored_payment_method_id"].template As<boost::uuids::uuid>();
    }
    payment_attempt.payment_method_id = value["payment_method_id"].template As<PaymentMethodId>();
    payment_attempt.amount = std::stoll(value["amount"].template As<std::string>());
    payment_attempt.status = value["status"].template As<PaymentAttemptStatus>();
    if (value.HasMember("error_code")) {
        payment_attempt.error_code = value["error_code"].template As<std::optional<PaymentAttemptErrorCode>>();
    }
    payment_attempt.method_details = value["method_details"].template As<PaymentMethod>();
    payment_attempt.created_at = value["created_at"].template As<Timestamp>();
    if (value.HasMember("captured_at") && !value["captured_at"].IsNull()) {
        payment_attempt.captured_at = value["captured_at"].template As<Timestamp>();
    }
    return payment_attempt;
}

// Checkout
template <typename Format>
Format Serialize(const Checkout& checkout, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["url"] = checkout.url;
    return builder.ExtractValue();
}

template <typename Value>
Checkout Parse(const Value& value, userver::formats::parse::To<Checkout>) {
    Checkout checkout;
    if (value.HasMember("url")) {
        checkout.url = value["url"].template As<std::optional<std::string>>();
    }
    return checkout;
}

// Transaction
template <typename Format>
Format Serialize(const Transaction& transaction, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["id"] = transaction.id;
    builder["status"] = transaction.status;
    builder["customer_id"] = transaction.customer_id;
    builder["address_id"] = transaction.address_id;
    builder["business_id"] = transaction.business_id;
    builder["custom_data"] = transaction.custom_data;
    builder["currency_code"] = transaction.currency_code;
    builder["origin"] = transaction.origin;
    builder["subscription_id"] = transaction.subscription_id;
    builder["invoice_id"] = transaction.invoice_id;
    builder["document_number"] = transaction.document_number;
    builder["collection_mode"] = transaction.collection_mode;
    builder["discount_id"] = transaction.discount_id;
    builder["billing_details"] = transaction.billing_details;
    builder["billing_period"] = transaction.billing_period;
    builder["items"] = transaction.items;
    builder["details"] = transaction.details;
    builder["payments"] = transaction.payments;
    builder["checkout"] = transaction.checkout;
    builder["created_at"] = transaction.created_at;
    builder["updated_at"] = transaction.updated_at;
    builder["billed_at"] = transaction.billed_at;
    builder["revised_at"] = transaction.revised_at;
    return builder.ExtractValue();
}

template <typename Value>
Transaction Parse(const Value& value, userver::formats::parse::To<Transaction>) {
    Transaction transaction;
    transaction.id = value["id"].template As<TransactionId>();
    transaction.status = value["status"].template As<TransactionStatus>();
    if (value.HasMember("customer_id")) {
        transaction.customer_id = value["customer_id"].template As<OptionalCustomerId>();
    }
    if (value.HasMember("address_id")) {
        transaction.address_id = value["address_id"].template As<OptionalAddressId>();
    }
    if (value.HasMember("business_id")) {
        transaction.business_id = value["business_id"].template As<OptionalBusinessId>();
    }
    transaction.custom_data = value["custom_data"].template As<JSON>();
    transaction.currency_code = value["currency_code"].template As<money::CurrencyCode>();
    transaction.origin = value["origin"].template As<TransactionOrigin>();
    if (value.HasMember("subscription_id")) {
        transaction.subscription_id = value["subscription_id"].template As<OptionalSubscriptionId>();
    }
    if (value.HasMember("invoice_id")) {
        transaction.invoice_id = value["invoice_id"].template As<std::optional<std::string>>();
    }
    if (value.HasMember("document_number")) {
        transaction.document_number = value["document_number"].template As<OptionalDocumentNumber>();
    }
    transaction.collection_mode = value["collection_mode"].template As<CollectionMode>();
    if (value.HasMember("discount_id")) {
        transaction.discount_id = value["discount_id"].template As<OptionalDiscountId>();
    }
    if (value.HasMember("billing_details")) {
        transaction.billing_details = value["billing_details"].template As<std::optional<BillingDetails>>();
    }
    if (value.HasMember("billing_period")) {
        transaction.billing_period = value["billing_period"].template As<std::optional<TimePeriod>>();
    }
    transaction.items = value["items"].template As<std::vector<Item>>();
    transaction.details = value["details"].template As<Details>();
    if (value.HasMember("payments")) {
        transaction.payments = value["payments"].template As<std::vector<PaymentAttempt>>();
    }
    if (value.HasMember("checkout")) {
        transaction.checkout = value["checkout"].template As<std::optional<Checkout>>();
    }
    transaction.created_at = value["created_at"].template As<Timestamp>();
    transaction.updated_at = value["updated_at"].template As<Timestamp>();
    if (value.HasMember("billed_at")) {
        transaction.billed_at = value["billed_at"].template As<OptionalTimestamp>();
    }
    if (value.HasMember("revised_at")) {
        transaction.revised_at = value["revised_at"].template As<OptionalTimestamp>();
    }
    return transaction;
}

}  // namespace paddle::transactions

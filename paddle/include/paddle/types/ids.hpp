#pragma once

#include <userver/utils/strong_typedef.hpp>

#include <boost/uuid/uuid.hpp>

namespace paddle {

using RequestId = userver::utils::StrongTypedef<struct RequestIdTag, boost::uuids::uuid>;

using ProductId = userver::utils::StrongTypedef<struct ProductIdTag, std::string>;
using OptionalProductId = std::optional<ProductId>;

using PriceId = userver::utils::StrongTypedef<struct PriceIdTag, std::string>;
using OptionalPriceId = std::optional<PriceId>;

using DiscountId = userver::utils::StrongTypedef<struct DiscountIdTag, std::string>;
using OptionalDiscountId = std::optional<DiscountId>;

using TransactionId = userver::utils::StrongTypedef<struct TransactionIdTag, std::string>;
using OptionalTransactionId = std::optional<TransactionId>;

using TransactionItemId = userver::utils::StrongTypedef<struct TransactionItemIdTag, std::string>;
using OptionalTransactionItemId = std::optional<TransactionItemId>;

using PaymentAttemptId = userver::utils::StrongTypedef<struct PaymentAttemptIdTag, boost::uuids::uuid>;
using OptionalPaymentAttemptId = std::optional<PaymentAttemptId>;

using SubscriptionId = userver::utils::StrongTypedef<struct SubscriptionIdTag, std::string>;
using OptionalSubscriptionId = std::optional<SubscriptionId>;

using CustomerId = userver::utils::StrongTypedef<struct CustomerIdTag, std::string>;
using OptionalCustomerId = std::optional<CustomerId>;

using Email = userver::utils::StrongTypedef<struct EmailTag, std::string>;
using OptionalEmail = std::optional<Email>;

using PaymentMethodId = userver::utils::StrongTypedef<struct PaymentMethodIdTag, std::string>;
using OptionalPaymentMethodId = std::optional<PaymentMethodId>;

using AddressId = userver::utils::StrongTypedef<struct AddressIdTag, std::string>;
using OptionalAddressId = std::optional<AddressId>;

using BusinessId = userver::utils::StrongTypedef<struct BusinessIdTag, std::string>;
using OptionalBusinessId = std::optional<BusinessId>;

using DocumentNumber = userver::utils::StrongTypedef<struct DocumentNumberTag, std::string>;
using OptionalDocumentNumber = std::optional<DocumentNumber>;

using NotificationSettingId = userver::utils::StrongTypedef<struct NotificationSettingIdTag, std::string>;
using ClientTokenId = userver::utils::StrongTypedef<struct ClientTokenIdTag, std::string>;
using ApiKeyId = userver::utils::StrongTypedef<struct ApiKeyIdTag, std::string>;

using EventId = userver::utils::StrongTypedef<struct EventIdTag, std::string>;
using NotificationId = userver::utils::StrongTypedef<struct NotificationIdTag, std::string>;

}  // namespace paddle

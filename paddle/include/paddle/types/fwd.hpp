#pragma once

namespace paddle {

namespace products {

template <typename T>
struct ProductTemplate;

}  // namespace products

namespace money {
struct PaymentMethodEventPayload;
struct SavedPaymentMethod;
}  // namespace money

namespace prices {

template <typename T>
struct PriceTemplate;

struct Adjustment;

}  // namespace prices

namespace customers {
struct Customer;
struct Address;
struct Contact;
struct Business;
}  // namespace customers

namespace subscriptions {
struct Subscription;
}  // namespace subscriptions

namespace transactions {
struct Transaction;
}

namespace events {
template <typename T>
struct EventWithNotification;

template <typename T>
struct Event;
}  // namespace events

namespace api_keys {
struct ApiKeyEventPayload;
}  // namespace api_keys

namespace client_tokens {
struct ClientToken;
}  // namespace client_tokens

struct NotificationSetting;

}  // namespace paddle

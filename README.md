# userver Paddle Integration Components

A comprehensive set of userver-compatible components for integrating with [Paddle Billing](https://www.paddle.com/billing), providing webhook handling, API client functionality, and event processing for subscription management.

## Features

- 🚀 **Paddle API Client** - Fetch products, prices, subscriptions, transactions, and events
- 🔐 **Secure Webhook Handling** - Automatic signature verification with cached webhook secrets  
- 📦 **Event-driven Architecture** - Modular event handlers for all Paddle entity types
- 🛡️ **Type Safety** - Comprehensive C++ type definitions for all Paddle entities
- 🗄️ **PostgreSQL Ready** - All types include PostgreSQL serialization support
- ⚡ **Performance Optimized** - Fast signature verification and cached webhook secrets
- 🔧 **userver Native** - Built specifically for userver framework with proper components

## Architecture Overview

The integration consists of three main layers:

1. **API Client Layer** (`components/client.*`) - Handles REST API calls to Paddle
2. **Webhook Layer** (`components/event_handlers/webhook_handler.*`) - Processes incoming webhooks
3. **Event Handler Layer** (`components/event_handlers/*_handler_base.*`) - Handles specific entity events

## Quick Start

### 1. Add Components to Your Service

```cpp
#include <paddle/components/client.hpp>
#include <paddle/components/webhook_secret_cache.hpp>
#include <paddle/components/event_handlers/webhook_handler.hpp>

// Add to your component list
ComponentList()
    .Append<paddle::components::Client>("paddle-client")
    .Append<paddle::components::WebhookSecretCache>("paddle-webhook-secrets")
    .Append<paddle::components::WebhookHandler>("/paddle/webhook");
```

### 2. Configure Components

```yaml
# Paddle API client
paddle-client:
    base_url: "https://api.paddle.com"  # or sandbox: "https://sandbox-api.paddle.com"
    api_key: !env PADDLE_API_KEY
    api_version: "1"
    webhook_host: "webhook.yourdomain.com"

# Webhook secret cache
paddle-webhook-secrets:
    update-period: 10m
    webhook_host: "webhook.yourdomain.com"
    max_signature_age_seconds: 300

# Webhook handler
/paddle/webhook:
    path: /paddle/webhook/*
    method: POST
    secrets_cache: paddle-webhook-secrets
    run_in_background: true
    # Configure which event handlers to use (optional)
    transactions: my-transaction-handler
    subscriptions: my-subscription-handler
    customers: my-customer-handler
```

### 3. Create Custom Event Handlers

```cpp
#include <paddle/components/event_handlers/transaction_handler_base.hpp>

class MyTransactionHandler : public paddle::components::TransactionHandlerBase {
public:
    using BaseType::BaseType;
    static constexpr std::string_view kName = "my-transaction-handler";

private:
    void DoHandleCompleted(EventType&& event) const override {
        LOG_INFO() << "Transaction completed: " << event.event_id 
                   << " amount: " << event.data.details.totals.total;
        // Your business logic here
    }
    
    void DoHandlePaid(EventType&& event) const override {
        // Handle payment completion
    }
    
    void DoHandlePaymentFailed(EventType&& event) const override {
        // Handle payment failure
    }
    
    // NOTE: Events without overridden handlers are only logged in short format:
    // "Event ignored: evt_123 transaction.billed 2023-08-16T18:20:25Z notification_id: ntf_456"
};
```

## Components Reference

### Client Component

The main API client for interacting with Paddle's REST API.

```cpp
#include <paddle/components/client.hpp>

class MyService {
    paddle::components::Client& client_;
    
public:
    void GetProducts() {
        auto products = client_.GetAllProducts();
        for (const auto& product : products) {
            LOG_INFO() << "Product: " << product.name;
        }
    }
    
    void GetSubscriptions() {
        // Paginated access
        auto response = client_.GetSubscriptions("", 50);
        for (const auto& sub : response.data) {
            LOG_INFO() << "Subscription: " << sub.id;
        }
    }
};
```

**Supported API Methods:**
- `GetAllProducts()` / `GetProducts(cursor, per_page)` - Fetch products
- `GetAllPrices()` / `GetPrices(cursor, per_page)` - Fetch pricing
- `GetAllSubscriptions()` / `GetSubscriptions(cursor, per_page)` - Fetch subscriptions
- `GetAllEvents()` / `GetEvents(cursor, per_page)` - Fetch events
- `GetAllNotificationSettings()` / `GetNotificationSettings(cursor, per_page)` - Fetch webhooks

### Webhook Handler

Processes incoming webhooks with automatic signature verification and event routing.

```cpp
// Webhook automatically routes to appropriate handlers based on event type
// POST /paddle/webhook -> WebhookHandler -> TransactionHandlerBase descendant (if configured)
```

**Features:**
- ✅ Automatic signature verification using cached webhook secrets
- ✅ Event type detection and routing to appropriate handlers
- ✅ Background processing support for non-blocking webhook responses
- ✅ Comprehensive error handling and logging

**Important:** Events are only processed if you have:
1. Configured a handler component for that event category
2. Overridden the specific event method in your handler class

Without custom handlers, events are logged in a minimal format and ignored.

You can override behavoiur only for events you really need.

### Event Handlers

Modular base classes for handling different entity types. **You must override specific event methods to handle them - otherwise they are only logged.**

| Handler | Events Handled | Use Cases |
|---------|---------------|-----------|
| `AddressHandlerBase` | `address.*` | Adress events |
| `ApiKeyHandlerBase` | `api_key.*` | API Key events |
| `BusinessHanderBase` | `business.*` | Business object changes |
| `ClientTokenHandlerBase` | `client_token.*` | Token creation, revokation |
| `CustomerHandlerBase` | `customer.*` | User account management |
| `PaymentMethodHandlerBase` | `payment_method.*` | Payment method updates |
| `PriceHandlerBase` | `price.*` | Pricing updates |
| `ProductHandlerBase` | `product.*` | Catalog management |
| `SubscriptionHandlerBase` | `subscription.*` | Subscription lifecycle, billing |
| `TransactionHandlerBase` | `transaction.*` | Payment processing, order fulfillment |

**Default Behavior:** If you don't override a handler method, the event will be logged as:
```
Event ignored: evt_01k2jjm0qdjr26zsz4m48z2efq transaction.completed 2025-08-16T18:20:25Z notification_id: ntf_01k2jjm13zz5m5t681nvn0e5hr
```

What's not implemented yet:
`adjustment.*`, `discount.*`, `discount_group.*`, `payout.*` and `report.*`

### Event Replay Controller

Fetches events from Paddle and replays them using the same handlers that process live webhook events.

```cpp
#include <paddle/components/event_replay_controller.hpp>

class MyService {
    paddle::components::EventReplayController& replay_controller_;
    
public:
    // Replay a single event
    void ReplaySingleEvent(paddle::events::Event<JSON>&& event) {
        replay_controller_.Replay(std::move(event));
    }
    
    // Replay all events since a cursor position
    void ReplayEventsSince(const std::string& cursor) {
        replay_controller_.ReplaySince(cursor, [](const auto& event) {
            LOG_INFO() << "Replaying event: " << event.event_id;
        });
    }
};
```

**Features:**
- 🔄 **Batch Processing** - Fetches events in batches (200 events per batch by default)
- 🎯 **Same Handler Logic** - Uses identical event handlers as webhook processing
- 📍 **Cursor Support** - Replay events from specific positions
- 🎭 **Event Categories** - Supports all event categories (transactions, subscriptions, etc.)
- ⚡ **CPU-friendly** - Built-in CPU relaxation during batch processing

**Supported Event Categories:**
- Transaction events (`transaction.*`)
- Subscription events (`subscription.*`) 
- Customer events (`customer.*`)
- Payment method events (`payment_method.*`)
- Price events (`price.*`)
- Product events (`product.*`)
- Address events (`address.*`)
- Business events (`business.*`)
- API key events (`api_key.*`)
- Client token events (`client_token.*`)

**Configuration:**
```yaml
event-replay-controller:
    client_name: paddle-client  # Name of Paddle client component (default: "paddle-client")
    # Event handler configurations (same as webhook handlers)
    transactions: my-transaction-handler
    subscriptions: my-subscription-handler
    customers: my-customer-handler
    # ... other handlers
```

**API Methods:**
- `Replay(event)` - Replay a single event through appropriate handlers
- `ReplaySince(cursor, callback)` - Replay all events since cursor position with optional progress callback

### Webhook Secret Cache

Automatically fetches and caches webhook endpoint secrets for signature verification.

**Features:**
- 🔄 Automatic periodic updates from Paddle API
- 🚀 Fast in-memory caching for webhook verification
- 🛡️ Secure signature validation with configurable max age
- 📊 Built-in metrics and monitoring

## Event Types

The components handle all Paddle webhook events. **You must override the methods to handle them:**

### Transaction Events
```cpp
class MyTransactionHandler : public paddle::components::TransactionHandlerBase {
    // Override any/all of these methods - unoverridden events are only logged
    void DoHandleBilled(EventType&& event) const override;        // transaction.billed
    void DoHandleCanceled(EventType&& event) const override;      // transaction.canceled  
    void DoHandleCompleted(EventType&& event) const override;     // transaction.completed
    void DoHandleCreated(EventType&& event) const override;       // transaction.created
    void DoHandlePaid(EventType&& event) const override;          // transaction.paid
    void DoHandlePastDue(EventType&& event) const override;       // transaction.past_due
    void DoHandlePaymentFailed(EventType&& event) const override; // transaction.payment_failed
    void DoHandleReady(EventType&& event) const override;         // transaction.ready
    void DoHandleRevised(EventType&& event) const override;       // transaction.revised
    void DoHandleUpdated(EventType&& event) const override;       // transaction.updated
};
```

### Subscription Events
```cpp
class MySubscriptionHandler : public paddle::components::SubscriptionHandlerBase {
    // Override any/all of these methods
    void DoHandleActivated(EventType&& event) const override;     // subscription.activated
    void DoHandleCanceled(EventType&& event) const override;      // subscription.canceled
    void DoHandleCreated(EventType&& event) const override;       // subscription.created
    void DoHandleImported(EventType&& event) const override;      // subscription.imported
    void DoHandlePastDue(EventType&& event) const override;       // subscription.past_due
    void DoHandlePaused(EventType&& event) const override;        // subscription.paused
    void DoHandleResumed(EventType&& event) const override;       // subscription.resumed
    void DoHandleUpdated(EventType&& event) const override;       // subscription.updated
    void DoHandleTrialing(EventType&& event) const override;      // subscription.trialing
};
```

### Customer Events
```cpp  
class MyCustomerHandler : public paddle::components::CustomerHandlerBase {
    void DoHandleCreated(EventType&& event) const override;       // customer.created
    void DoHandleImported(EventType&& event) const override;      // customer.imported
    void DoHandleUpdated(EventType&& event) const override;       // customer.updated
};
```

*Similar patterns for products, prices, payment methods, addresses, businesses, API keys, and client tokens.*

## Type System

Comprehensive C++ types for all Paddle entities with full serialization and PostgreSQL support:

### Core Types
```cpp
namespace paddle {
    // Strong typed IDs - all PostgreSQL compatible
    using TransactionId = userver::utils::StrongTypedef<struct TransactionIdTag, std::string>;
    using SubscriptionId = userver::utils::StrongTypedef<struct SubscriptionIdTag, std::string>;
    using CustomerId = userver::utils::StrongTypedef<struct CustomerIdTag, std::string>;
    
    // Money handling
    namespace money {
        struct Money {
            std::int32_t amount;           // In smallest currency unit (cents)
            CurrencyCode currency_code;    // ISO 4217 code
            
            // PostgreSQL serialization included
            bool operator==(const Money& other) const;
        };
    }
    
    // Comprehensive entity types with PostgreSQL support
    namespace transactions {
        struct Transaction {
            TransactionId id;
            TransactionStatus status;      // Mapped to PostgreSQL enum
            Details details;               // Totals, line items, etc.
            std::vector<PaymentAttempt> payment_attempts;
            Timestamp created_at;          // PostgreSQL TIMESTAMPTZ
            Timestamp updated_at;
            // ... full transaction data
        };
    }
}
```

### Event Structure
```cpp
namespace paddle::events {
    template<typename T>
    struct Event {
        EventId event_id;
        EventTypeName event_type;    // PostgreSQL enum mapped
        Timestamp occurred_at;       // PostgreSQL TIMESTAMPTZ
        T data;                      // Transaction, Subscription, etc.
    };
    
    // For webhook events
    template<typename T> 
    struct EventWithNotification : Event<T> {
        NotificationId notification_id;
    };
}
```

## Configuration Reference

### Environment Variables

```bash
# Required
PADDLE_API_KEY=your_paddle_api_key_here

# Optional
PADDLE_ENVIRONMENT=sandbox  # or "production"  
PADDLE_WEBHOOK_HOST=webhook.yourdomain.com
```

### Full Configuration Example

```yaml
# config/config.yaml
components_manager:
    components:
        # PostgreSQL cluster
        postgres-db-1:
            dbalias: postgresql://user:password@localhost:5432/paddle_db
            blocking_task_processor: fs-task-processor
            dns_resolver: async
            
        # Paddle API client
        paddle-client:
            base_url: !env PADDLE_API_URL "https://sandbox-api.paddle.com"
            api_key: !env PADDLE_API_KEY
            api_version: 1
            webhook_host: !env PADDLE_WEBHOOK_HOST "webhook.yourdomain.com"
        
        # Webhook secret cache  
        paddle-webhook-secrets:
            update-period: 10m
            webhook_host: !env PADDLE_WEBHOOK_HOST "webhook.yourdomain.com"
            max_signature_age_seconds: 300
            
        # Custom event handlers
        my-transaction-handler:
            # Your custom configuration
            
        my-subscription-handler:
            # Your custom configuration
            
        # Webhook endpoint
        handlder-paddle-webhook:
            path: /paddle/webhook/*
            method: POST
            secrets_cache: paddle-webhook-secrets
            run_in_background: true
            # Wire up your custom handlers
            transactions: my-transaction-handler
            subscriptions: my-subscription-handler
```

## Usage Examples

### Basic API Usage

```cpp
#include <paddle/components/client.hpp>

class BillingService : public userver::components::ComponentBase {
    paddle::components::Client& paddle_client_;
    
public:
    BillingService(const ComponentConfig& config, const ComponentContext& context)
        : ComponentBase(config, context)
        , paddle_client_(context.FindComponent<paddle::components::Client>()) {}
    
    // Get customer's active subscriptions
    std::vector<paddle::subscriptions::Subscription> GetCustomerSubscriptions(
        const std::string& customer_id
    ) {
        auto all_subs = paddle_client_.GetAllSubscriptions();
        std::vector<paddle::subscriptions::Subscription> customer_subs;
        
        std::copy_if(all_subs.begin(), all_subs.end(), 
                    std::back_inserter(customer_subs),
                    [&](const auto& sub) { 
                        return sub.customer_id == customer_id && 
                               sub.status == paddle::SubscriptionStatus::kActive; 
                    });
                    
        return customer_subs;
    }
};
```

## PostgreSQL Integration

| TODO Provide a schema and queries for storing Paddle data locally.

All Paddle types include full PostgreSQL support with automatic enum mapping:

```cpp
// All enums are mapped to PostgreSQL enum types
template <>
struct userver::storages::postgres::io::CppToUserPg<paddle::events::EventTypeName>
    : storages::postgres::io::EnumMappingBase<paddle::events::EventTypeName> {
    static constexpr DBTypeName postgres_name = "paddle.event_type_name";
    // Maps C++ enums to PostgreSQL enum values
};

// Store events directly in PostgreSQL
void StoreEvent(const paddle::events::Event<paddle::transactions::Transaction>& event) {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "INSERT INTO paddle_events (event_id, event_type, occurred_at, data) "
        "VALUES ($1, $2, $3, $4)",
        event.event_id,
        event.event_type,     // Automatically converted to PostgreSQL enum
        event.occurred_at,
        userver::formats::json::ToString(userver::formats::json::ValueBuilder(event.data).ExtractValue())
    );
}
```

**PostgreSQL Schema Example:**
```sql
-- Create enum types
CREATE TYPE paddle.event_type_name AS ENUM (
    'transaction.billed',
    'transaction.completed', 
    'transaction.created',
    'subscription.activated',
    'subscription.canceled',
    -- ... all event types
);

CREATE TYPE paddle.subscription_status AS ENUM (
    'active',
    'canceled', 
    'past_due',
    'paused',
    'trialing'
);

-- Store events with proper typing
CREATE TABLE paddle_events (
    id SERIAL PRIMARY KEY,
    event_id TEXT NOT NULL UNIQUE,
    event_type paddle.event_type_name NOT NULL,
    occurred_at TIMESTAMPTZ NOT NULL,
    notification_id TEXT,
    data JSONB NOT NULL,
    processed_at TIMESTAMPTZ DEFAULT NOW()
);

-- Store subscriptions
CREATE TABLE paddle_subscriptions (
    id TEXT PRIMARY KEY,
    status paddle.subscription_status NOT NULL,
    customer_id TEXT NOT NULL,
    created_at TIMESTAMPTZ NOT NULL,
    updated_at TIMESTAMPTZ NOT NULL,
    -- ... other fields
);
```

### PostgreSQL Integration Example

```cpp
#include <paddle/components/event_handlers/transaction_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

class TransactionPersister : public paddle::components::TransactionHandlerBase {
    userver::storages::postgres::Cluster& pg_cluster_;

public:
    static constexpr std::string_view kName = "transaction-persister";
    
    TransactionPersister(const ComponentConfig& config, const ComponentContext& context)
        : BaseType(config, context)
        , pg_cluster_(context.FindComponent<userver::storages::postgres::Cluster>()) {}

private:
    void DoHandleCompleted(EventType&& event) const override {
        // Store transaction in PostgreSQL with automatic type conversion
        auto result = pg_cluster_.Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "INSERT INTO paddle_transactions "
            "(id, status, customer_id, total_amount, currency, created_at, updated_at) "
            "VALUES ($1, $2, $3, $4, $5, $6, $7) "
            "ON CONFLICT (id) DO UPDATE SET "
            "status = EXCLUDED.status, updated_at = EXCLUDED.updated_at",
            event.data.id,
            event.data.status,                                    // Auto-converts enum
            event.data.customer_id,
            event.data.details.totals.total,
            event.data.details.totals.currency_code,
            event.data.created_at,                               // Auto-converts timestamp
            event.data.updated_at
        );
        
        // Also store the raw event
        pg_cluster_.Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "INSERT INTO paddle_events (event_id, event_type, occurred_at, data) "
            "VALUES ($1, $2, $3, $4)",
            event.event_id,
            event.event_type,                                    // Auto-converts enum
            event.occurred_at,
            userver::formats::json::ToString(
                userver::formats::json::ValueBuilder(event.data).ExtractValue()
            )
        );
        
        LOG_INFO() << "Stored completed transaction: " << event.data.id;
    }
    
    void DoHandlePaid(EventType&& event) const override {
        // Update payment status
        pg_cluster_.Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "UPDATE paddle_transactions SET status = $1, updated_at = $2 WHERE id = $3",
            paddle::TransactionStatus::kCompleted,              // PostgreSQL enum
            std::chrono::system_clock::now(),
            event.data.id
        );
    }
    
    // NOTE: DoHandleCreated, DoHandleBilled, etc. not overridden
    // Those events will only be logged: "Event ignored: evt_123 transaction.created ..."
};
```

### Advanced Event Handling with PostgreSQL

```cpp
class SubscriptionManager : public paddle::components::SubscriptionHandlerBase {
    userver::storages::postgres::Cluster& pg_cluster_;

public:
    static constexpr std::string_view kName = "subscription-manager";
    using BaseType::BaseType;

private:
    void DoHandleActivated(EventType&& event) const override {
        auto& subscription = event.data;
        
        // Store subscription with PostgreSQL enum types
        auto result = pg_cluster_.Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "INSERT INTO paddle_subscriptions "
            "(id, status, customer_id, created_at, current_billing_period) "
            "VALUES ($1, $2, $3, $4, $5) "
            "ON CONFLICT (id) DO UPDATE SET status = EXCLUDED.status",
            subscription.id,
            subscription.status,                    // Automatic PostgreSQL enum conversion
            subscription.customer_id,
            subscription.created_at,
            subscription.current_billing_period
        );
        
        // Unlock premium features
        pg_cluster_.Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "UPDATE user_accounts SET premium_until = $1 WHERE customer_id = $2",
            subscription.current_billing_period.ends_at,
            subscription.customer_id
        );
        
        LOG_INFO() << "Subscription activated and stored: " << subscription.id;
    }
    
    void DoHandleCanceled(EventType&& event) const override {
        // Update subscription status using PostgreSQL enum
        pg_cluster_.Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "UPDATE paddle_subscriptions SET status = $1, updated_at = NOW() WHERE id = $2",
            paddle::SubscriptionStatus::kCanceled,    // PostgreSQL enum
            event.data.id
        );
        
        LOG_WARNING() << "Subscription canceled: " << event.data.id;
    }
    
    // NOTE: Methods like DoHandleCreated, DoHandleImported not overridden
    // Those events will be logged but not processed
};
```

## Testing

### Unit Tests

The library includes comprehensive unit tests:

```cpp
// tests/signature_test.cpp - Test signature verification
// tests/money_test.cpp - Test currency handling  
// tests/products_test.cpp - Test product serialization
// tests/subscription_test.cpp - Test subscription events
```

Run tests:
```bash
mkdir build && cd build
cmake ..
make paddle_unittest
./paddle_unittest
```

### Integration Testing

For webhook testing, use Paddle's webhook simulation:

1. Set up webhook endpoint: `POST /paddle/webhook`
2. Configure in Paddle Dashboard with your webhook URL
3. Use Paddle's webhook testing tool to send test events
4. Verify events are processed correctly in your handlers

### Signature Verification Testing

```cpp
#include <paddle/auth/signature.hpp>

TEST(Paddle, SignatureVerification) {
    const auto secret = "pdl_ntfset_01k2jjfqx34sdwsvrbj123wxx2_w4C4I+q1LScX16/ODlt39IBfOQo+20fN";
    const auto payload = R"({"event_id":"evt_123","event_type":"transaction.completed"})";
    const auto signature = "ts=1755117651;h1=cf519461c15c010f1a82e28afc83b7e8a5fdf1823791050e775badbe0bd cabf7";
    
    ASSERT_TRUE(paddle::VerifySignature(secret, signature, payload));
    ASSERT_FALSE(paddle::VerifySignature(secret, signature, payload, 0)); // Max age 0
}
```

## Event Handler Behavior

**Important:** Events are only processed if you override the specific handler methods. The default behavior is:

1. **With Handler Configured + Method Overridden**: Full event processing with your business logic
2. **With Handler Configured + Method NOT Overridden**: Event logged in short format and ignored
3. **No Handler Configured**: Event logged with category warning

```cpp
// Example log output for unhandled events:
// INFO: Event ignored: evt_01k2jj transaction.billed 2025-08-16T18:20:25Z notification_id: ntf_01k2jj
// INFO: Event handling not implemented for event category: discount
```

To handle events, you must:
1. Create a handler class extending the appropriate base
2. Configure it in your webhook handler YAML
3. Override the specific `DoHandle*` methods you need

## Security

### Webhook Security

- **Signature Verification**: All webhooks are automatically verified using HMAC-SHA256
- **Secret Rotation**: Webhook secrets are automatically fetched and cached
- **Timestamp Validation**: Configurable maximum age for webhook signatures
- **HTTPS Only**: Use HTTPS endpoints in production

### API Security

- **API Key Management**: Store API keys securely using environment variables
- **Rate Limiting**: Respect Paddle's API rate limits
- **Error Handling**: Proper error handling prevents sensitive information leakage

## Performance

### Optimizations

- **Fast signature verification** using optimized HMAC implementation
- **Cached webhook secrets** to avoid API calls on every webhook
- **Background processing** option for non-blocking webhook handling
- **Efficient JSON parsing** with userver's fast JSON implementation
- **PostgreSQL connection pooling** for database operations

### Monitoring

Built-in metrics for:
- Webhook processing time
- Signature verification success/failure rates  
- API request latencies
- Cache hit rates for webhook secrets

## Production Checklist

- [ ] Configure HTTPS webhook endpoints
- [ ] Set up proper API key management
- [ ] Configure webhook secret caching intervals
- [ ] Set up monitoring and alerting
- [ ] Test signature verification thoroughly
- [ ] Implement proper error handling in event handlers
- [ ] Configure appropriate background processing
- [ ] Set up PostgreSQL database with proper enum types
- [ ] Test PostgreSQL serialization/deserialization
- [ ] Configure logging levels appropriately
- [ ] Override all event handler methods you need

## Migration from Paddle Classic

If migrating from Paddle Classic (v3 API):

1. **Update API endpoints**: Use Paddle Billing API URLs
2. **Update webhook signatures**: Use new HMAC-SHA256 signature format  
3. **Update event structure**: Events now have `event_id` and `occurred_at`
4. **Update entity IDs**: New ID format (e.g., `txn_01gm302t81w94gyjpjpqypkzkf`)
5. **Review entity schemas**: Some fields have changed names/formats
6. **Update PostgreSQL schema**: New enum types and field mappings

## Troubleshooting

### Common Issues

**Webhook signature verification failing:**
```
- Check webhook secret is correct
- Verify timestamp is within max_signature_age_seconds
- Ensure payload hasn't been modified
- Check HTTPS is used in production
```

**Events not being processed:**
```
- Verify event handler is configured in webhook handler YAML
- Check that you've overridden the specific DoHandle* method
- Look for "Event ignored" logs - these indicate unhandled events
- Verify component dependencies are correct
```

**PostgreSQL serialization errors:**
```
- Ensure enum types are created in database
- Check that enum values match C++ definitions
- Verify timestamp columns use TIMESTAMPTZ type
- Check connection pool configuration
```

### Debug Logging

Enable debug logging:
```cpp
LOG_DEBUG() << "Received event: " << event.event_type 
            << " id: " << event.event_id;
LOG_DEBUG() << "Event data: " << userver::formats::json::ToString(
                userver::formats::json::ValueBuilder(event.data).ExtractValue());
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Add tests for new functionality  
4. Ensure all tests pass
5. Submit a pull request

## License

[Specify your license here]

## Support

- **Documentation**: [Paddle Developer Docs](https://developer.paddle.com)
- **API Reference**: [Paddle API Reference](https://developer.paddle.com/api-reference)
- **Issues**: Create GitHub issues for bugs/features
- **Community**: Join the userver community discussions

---

*This integration provides a production-ready foundation for Paddle Billing in userver applications. The modular design allows you to implement only the event handlers you need while providing comprehensive type safety, PostgreSQL integration, and proper error handling.*

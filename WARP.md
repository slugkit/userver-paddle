# WARP.md

This file provides guidance to WARP (warp.dev) when working with code in this repository.

## Project Overview

The `userver-paddle` library is a comprehensive userver-compatible integration with Paddle Billing API. It provides webhook handling, API client functionality, and event processing for subscription management systems.

This is a **submodule library** within the larger `human-readable-id` project, designed to be included in userver applications that need Paddle Billing integration.

## Architecture

The library follows a three-layer architecture:

1. **API Client Layer** (`components/client.*`) - REST API calls to Paddle
2. **Webhook Layer** (`handlers/webhook_handler.*`) - Processes incoming webhooks with signature verification
3. **Event Handler Layer** (`handlers/*_handler_base.*`) - Modular handlers for specific entity events

### Key Components

- **Types System** (`include/paddle/types/`) - Comprehensive C++ type definitions for all Paddle entities with PostgreSQL serialization
- **Authentication** (`auth/signature.*`) - HMAC-SHA256 webhook signature verification
- **Components** (`components/`) - userver components for client, caching, and event processing
- **Handlers** (`handlers/`) - Base classes for handling different Paddle event categories

### Event-Driven Design

The library uses an inheritance-based event handling pattern:
- Base handler classes (e.g., `TransactionHandlerBase`) define event method signatures
- Custom handlers inherit from base classes and override specific event methods (e.g., `DoHandleCompleted`)
- Unoverridden methods result in events being logged but not processed
- Event routing is automatic based on event type and configured handlers

## Common Development Tasks

### Building the Library

The library is designed to be built as part of a larger userver project:

```bash
# Configure and build (requires userver to be available)
mkdir build && cd build
cmake .. -DUSERVER_PADDLE_BUILD_TESTS=ON -DUSERVER_PADDLE_BUILD_EXAMPLES=ON
make -j$(nproc)
```

### Running Tests

```bash
# Run unit tests
cd build
./paddle/paddle_unittest

# Run specific test suites
./paddle/paddle_unittest --gtest_filter="*Signature*"
./paddle/paddle_unittest --gtest_filter="*Money*"
```

### Building Examples

```bash
# Build with examples enabled
cmake .. -DUSERVER_PADDLE_BUILD_EXAMPLES=ON
make -j$(nproc)

# Run webhook example
./examples/webhook/webhook_service_example --config examples/webhook/static_config.yaml
```

### Development Environment Setup

```bash
# Required environment variables for development
export PADDLE_API_KEY="your_paddle_api_key_here"
export PADDLE_WEBHOOK_HOST="webhook.yourdomain.com"
export PADDLE_ENVIRONMENT="sandbox"  # or "production"
```

### Code Formatting

```bash
# Format C++ code (from project root)
clang-format -i $(find . -name "*.cpp" -o -name "*.hpp")
```

## Testing Strategy

### Unit Tests Structure

- `tests/signature_test.cpp` - HMAC signature verification
- `tests/money_test.cpp` - Currency/monetary calculations  
- `tests/products_test.cpp` - Product serialization/deserialization
- `tests/subscription_test.cpp` - Subscription event handling
- `tests/client_token_test.cpp` - Client token management
- `tests/notification_settings_test.cpp` - Webhook configuration

### Integration Testing

For webhook integration testing:
1. Configure webhook endpoint in Paddle Dashboard
2. Use Paddle's webhook testing tools to send test events
3. Verify signature verification and event processing
4. Test with various event types and malformed payloads

## Important Patterns

### Event Handler Implementation

When creating custom event handlers, follow this pattern:

```cpp
class CustomTransactionHandler : public paddle::components::TransactionHandlerBase {
public:
    static constexpr std::string_view kName = "custom-transaction-handler";
    using BaseType::BaseType;

private:
    // Override ONLY the events you need to handle
    void DoHandleCompleted(EventType&& event) const override {
        // Your business logic here
        // Event is automatically logged if not overridden
    }
};
```

### PostgreSQL Integration

All types include PostgreSQL serialization with enum mapping:
- Enums are mapped to PostgreSQL enum types
- Timestamps use `TIMESTAMPTZ`
- Money types handle currency precision correctly
- Strong typed IDs prevent mixing different entity types

### Configuration Pattern

Components follow userver configuration patterns:
```yaml
component-name:
    load-enabled: true
    option1: value1
    option2: value2
```

## Security Considerations

- All webhook signatures are automatically verified using HMAC-SHA256
- Webhook secrets are cached and periodically refreshed
- Configurable maximum signature age for replay attack prevention
- API keys should be stored in environment variables, never in code
- HTTPS is required for production webhook endpoints

## Performance Notes

- Webhook secret caching reduces API calls during verification
- Background processing option for non-blocking webhook handling
- Event handlers can be disabled to reduce processing overhead
- PostgreSQL connection pooling for database operations
- Fast JSON parsing with userver's optimized implementation

## Troubleshooting Common Issues

### Webhook Signature Verification Failures
- Check webhook secret configuration
- Verify timestamp is within `max_signature_age_seconds`
- Ensure payload hasn't been modified in transit
- Confirm HTTPS is used in production

### Events Not Being Processed
- Verify event handler is configured in webhook handler YAML
- Check that specific `DoHandle*` method is overridden
- Look for "Event ignored" logs indicating unhandled events
- Verify component dependencies are correct

### Build Issues
- Ensure userver is properly installed and available
- Check CMAKE_PREFIX_PATH includes userver cmake directory
- Verify compiler supports C++20 features
- Ensure all required userver components are available

## Dependencies

- **userver** - Core framework (universal, core components)
- **userver postgresql** - For PostgreSQL integration (optional)
- **fmt** - String formatting
- **clang++** - C++20 compatible compiler

## File Organization

- `include/paddle/` - Public headers organized by component type
- `src/paddle/` - Implementation files mirroring header structure  
- `tests/` - Unit tests for each major component
- `examples/` - Complete example applications showing usage patterns
- `CMakeLists.txt` - Build configuration with options for tests/examples

## Integration Notes

When integrating into a larger userver project:
1. Add as subdirectory with `add_subdirectory(libs/userver-paddle/paddle)`
2. Link against `paddle_client` target
3. Include required headers: `#include <paddle/components/client.hpp>`
4. Configure components in your YAML config
5. Implement custom event handlers inheriting from base classes
cmake_minimum_required(VERSION 3.20)
project(paddle_client CXX)

set(PADDLE_SRC
    include/paddle/auth/signature.hpp
    
    include/paddle/types/ids.hpp
    include/paddle/types/enums.hpp
    
    include/paddle/types/duration.hpp
    include/paddle/types/events.hpp
    include/paddle/types/formats.hpp
    include/paddle/types/money.hpp
    include/paddle/types/payment_method.hpp
    include/paddle/types/price.hpp
    include/paddle/types/product.hpp
    include/paddle/types/response.hpp
    include/paddle/types/transactions.hpp
    include/paddle/types/subscriptions.hpp
    include/paddle/types/client_token.hpp
    
    include/paddle/components/client.hpp
    include/paddle/components/webhook_secret_cache.hpp
    include/paddle/components/event_replay_controller.hpp
    include/paddle/components/price_cache.hpp
    include/paddle/components/product_cache.hpp

    include/paddle/components/price_cache.hpp
    include/paddle/handlers/transaction_handler_base.hpp
    include/paddle/handlers/subscription_handler_base.hpp
    include/paddle/handlers/customer_handler_base.hpp
    include/paddle/handlers/payment_method_handler_base.hpp
    include/paddle/handlers/price_handler_base.hpp
    include/paddle/handlers/product_handler_base.hpp
    include/paddle/handlers/address_handler_base.hpp
    include/paddle/handlers/business_handler_base.hpp
    include/paddle/handlers/api_key_handler_base.hpp
    include/paddle/handlers/client_token_handler_base.hpp
    include/paddle/handlers/handlers.hpp

    include/paddle/handlers/webhook_handler.hpp
    include/paddle/handlers/event_sink_base.hpp
    include/paddle/handlers/event_dispatcher.hpp

    src/paddle/auth/signature.cpp
    
    src/paddle/types/events.cpp
    src/paddle/types/payment_method.cpp
    src/paddle/types/price.cpp
    src/paddle/types/product.cpp
    src/paddle/types/transactions.cpp
    src/paddle/types/subscriptions.cpp
    src/paddle/types/client_token.cpp

    src/paddle/components/client.cpp
    
    src/paddle/components/webhook_secret_cache.cpp
    src/paddle/components/event_replay_controller.cpp
    src/paddle/components/price_cache.cpp
    src/paddle/components/product_cache.cpp

    src/paddle/handlers/transaction_handler_base.cpp
    src/paddle/handlers/subscription_handler_base.cpp
    src/paddle/handlers/customer_handler_base.cpp
    src/paddle/handlers/payment_method_handler_base.cpp
    src/paddle/handlers/price_handler_base.cpp
    src/paddle/handlers/product_handler_base.cpp
    src/paddle/handlers/address_handler_base.cpp
    src/paddle/handlers/business_handler_base.cpp
    src/paddle/handlers/api_key_handler_base.cpp
    src/paddle/handlers/client_token_handler_base.cpp
    src/paddle/handlers/handlers.cpp
    
    src/paddle/handlers/webhook_handler.cpp
    src/paddle/handlers/event_dispatcher.cpp

)

add_library(paddle_client OBJECT ${PADDLE_SRC})
target_link_libraries(paddle_client PRIVATE userver::core userver::postgresql)
target_include_directories(paddle_client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(paddle_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(USERVER_PADDLE_BUILD_TESTS)
add_executable(
    paddle_unittest
    tests/money_test.cpp
    tests/products_test.cpp
    tests/signature_test.cpp
    tests/subscription_test.cpp
    tests/notification_settings_test.cpp
    tests/client_token_test.cpp
)
target_link_libraries(paddle_unittest PRIVATE paddle_client userver::utest)
target_include_directories(paddle_unittest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_google_tests(paddle_unittest)
endif()

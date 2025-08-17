add_executable(webhook_example main.cpp)
target_link_libraries(webhook_example
    PRIVATE 
    userver::core
    paddle_client
)

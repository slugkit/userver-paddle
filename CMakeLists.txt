# This is a very simplified CMakeLists.txt for the userver-paddle project
# It is intended to be included to a bigger project with userver already 
# configured.

cmake_minimum_required(VERSION 3.20...3.31)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_C_COMPILER "/usr/bin/clang")

project(userver-paddle VERSION 0.1.0 LANGUAGES CXX)

option(USERVER_PADDLE_BUILD_POSTGRES "Build with postgres" ON)
option(USERVER_PADDLE_BUILD_TESTS "Build tests" ON)
option(USERVER_PADDLE_BUILD_EXAMPLES "Build test service" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_PREFIX_PATH "/userver/cmake")

include(/userver/cmake/ModuleHelpers.cmake)
include(/userver/cmake/UserverSetupEnvironment.cmake)

find_package(fmt REQUIRED)
userver_setup_environment()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
add_compile_options(-Wno-unknown-warning-option)

set(USERVER_PADDLE_COMPONENTS universal core)
if(USERVER_PADDLE_BUILD_POSTGRES)
  list(APPEND USERVER_PADDLE_COMPONENTS postgresql)
endif()

find_package(userver REQUIRED COMPONENTS ${USERVER_PADDLE_COMPONENTS})

add_subdirectory(paddle)

if(USERVER_PADDLE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

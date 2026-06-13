cmake_minimum_required(VERSION 3.25)
project(market_terminal VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(market_terminal src/main.cpp)
target_include_directories(market_terminal PRIVATE include)
target_link_libraries(market_terminal Threads::Threads)

include(FetchContent)
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG v1.14.0)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(terminal_tests tests/test_terminal.cpp)
target_include_directories(terminal_tests PRIVATE include)
target_link_libraries(terminal_tests GTest::gtest_main Threads::Threads)
include(GoogleTest)
gtest_discover_tests(terminal_tests)

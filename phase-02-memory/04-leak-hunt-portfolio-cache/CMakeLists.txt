cmake_minimum_required(VERSION 3.25)
project(portfolio_cache VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SANITIZE "Enable Address Sanitizer" OFF)

add_executable(portfolio_cache src/main.cpp)
target_include_directories(portfolio_cache PRIVATE include)

if(SANITIZE)
    target_compile_options(portfolio_cache PRIVATE -fsanitize=address -g)
    target_link_options(portfolio_cache    PRIVATE -fsanitize=address)
endif()

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(portfolio_cache_tests tests/test_portfolio_cache.cpp)
target_include_directories(portfolio_cache_tests PRIVATE include)
target_link_libraries(portfolio_cache_tests GTest::gtest_main)

if(SANITIZE)
    target_compile_options(portfolio_cache_tests PRIVATE -fsanitize=address -g)
    target_link_options(portfolio_cache_tests    PRIVATE -fsanitize=address)
endif()

include(GoogleTest)
gtest_discover_tests(portfolio_cache_tests)

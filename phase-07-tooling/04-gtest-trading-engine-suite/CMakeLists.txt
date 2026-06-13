cmake_minimum_required(VERSION 3.25)
project(engine_test_suite VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(trading_engine src/main.cpp)
target_include_directories(trading_engine PRIVATE include)

include(FetchContent)
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG v1.14.0)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(engine_tests
    tests/test_fixtures.cpp
    tests/test_parameterized.cpp
    tests/test_mocks.cpp
    tests/test_matchers.cpp
)
target_include_directories(engine_tests PRIVATE include)
target_link_libraries(engine_tests GTest::gtest_main GTest::gmock_main)
include(GoogleTest)
gtest_discover_tests(engine_tests)

cmake_minimum_required(VERSION 3.25)
project(risk_engine VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(risk_engine src/main.cpp)
target_include_directories(risk_engine PRIVATE include)
target_link_libraries(risk_engine Threads::Threads)

include(FetchContent)
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG v1.14.0)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(risk_tests tests/test_risk_engine.cpp)
target_include_directories(risk_tests PRIVATE include)
target_link_libraries(risk_tests GTest::gtest_main Threads::Threads)
include(GoogleTest)
gtest_discover_tests(risk_tests)

cmake_minimum_required(VERSION 3.25)
project(quant_platform VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(quant_platform src/main.cpp)
target_include_directories(quant_platform PRIVATE include)
target_link_libraries(quant_platform Threads::Threads)

include(FetchContent)
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG v1.14.0)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(platform_tests tests/test_platform.cpp)
target_include_directories(platform_tests PRIVATE include)
target_link_libraries(platform_tests GTest::gtest_main GTest::gmock_main Threads::Threads)
include(GoogleTest)
gtest_discover_tests(platform_tests)

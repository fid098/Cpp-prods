cmake_minimum_required(VERSION 3.25)
project(monte_carlo VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(monte_carlo src/main.cpp)
target_include_directories(monte_carlo PRIVATE include)
target_link_libraries(monte_carlo Threads::Threads)

include(FetchContent)
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG v1.14.0)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(mc_tests tests/test_mc.cpp)
target_include_directories(mc_tests PRIVATE include)
target_link_libraries(mc_tests GTest::gtest_main Threads::Threads)
include(GoogleTest)
gtest_discover_tests(mc_tests)

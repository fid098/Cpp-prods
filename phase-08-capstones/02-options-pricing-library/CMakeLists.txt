cmake_minimum_required(VERSION 3.25)
project(options_pricer VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(options_pricer src/main.cpp)
target_include_directories(options_pricer PRIVATE include)
target_link_libraries(options_pricer Threads::Threads)

include(FetchContent)
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG v1.14.0)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(options_tests tests/test_options.cpp)
target_include_directories(options_tests PRIVATE include)
target_link_libraries(options_tests GTest::gtest_main Threads::Threads)
include(GoogleTest)
gtest_discover_tests(options_tests)

cmake_minimum_required(VERSION 3.25)
project(order_book_memory_map VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(order_book_memory_map src/main.cpp)
target_include_directories(order_book_memory_map PRIVATE include)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(order_book_tests tests/test_order_entry.cpp)
target_include_directories(order_book_tests PRIVATE include)
target_link_libraries(order_book_tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(order_book_tests)

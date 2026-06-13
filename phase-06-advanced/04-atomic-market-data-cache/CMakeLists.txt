cmake_minimum_required(VERSION 3.25)
project(atomic_cache VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(atomic_cache src/main.cpp)
target_include_directories(atomic_cache PRIVATE include)
target_link_libraries(atomic_cache Threads::Threads)

include(FetchContent)
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG v1.14.0)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(atomic_cache_tests tests/test_atomic_cache.cpp)
target_include_directories(atomic_cache_tests PRIVATE include)
target_link_libraries(atomic_cache_tests GTest::gtest_main Threads::Threads)
include(GoogleTest)
gtest_discover_tests(atomic_cache_tests)

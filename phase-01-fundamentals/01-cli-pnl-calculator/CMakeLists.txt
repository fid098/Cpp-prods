cmake_minimum_required(VERSION 3.25)
project(pnl_calculator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Main executable ──────────────────────────────────────────────────────────
add_executable(pnl_calculator src/main.cpp)
target_include_directories(pnl_calculator PRIVATE include)

# ── Tests ────────────────────────────────────────────────────────────────────
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(pnl_calculator_tests tests/test_pnl.cpp)
target_include_directories(pnl_calculator_tests PRIVATE include)
target_link_libraries(pnl_calculator_tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(pnl_calculator_tests)

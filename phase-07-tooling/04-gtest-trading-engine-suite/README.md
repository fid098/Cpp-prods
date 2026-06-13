# 04 — GTest Trading Engine Suite

## What You'll Build

A **professional-grade test suite** for a small trading engine. This project teaches every GTest and GMock feature a senior C++ engineer is expected to know: fixtures, parameterized tests, death tests, custom matchers, and mock objects.

The trading engine under test handles orders, positions, and risk limits.

## Learning Objectives

| GTest Feature | Use Case |
|---------------|----------|
| `TEST_F` fixture | Shared setup/teardown for order book tests |
| `EXPECT_*` vs `ASSERT_*` | Non-fatal vs fatal assertions |
| `EXPECT_THROW` / `EXPECT_DEATH` | Exception and abort testing |
| `TYPED_TEST` | Testing templates over multiple numeric types |
| `TEST_P` + `INSTANTIATE_TEST_SUITE_P` | Data-driven / parameterized tests |
| `EXPECT_THAT` + matchers | Expressive container assertions |
| GMock `MOCK_METHOD` | Mocking interfaces (risk limit checker) |
| GMock `EXPECT_CALL` | Verifying interaction contracts |
| Custom matcher | `IsValidOrder()` |

## Project Structure

```
04-gtest-trading-engine-suite/
├── README.md
├── TUTORIAL.md
├── CMakeLists.txt
├── include/
│   ├── trading_engine.hpp
│   └── risk_checker.hpp
├── src/
│   └── main.cpp
└── tests/
    ├── test_fixtures.cpp
    ├── test_parameterized.cpp
    ├── test_mocks.cpp
    └── test_matchers.cpp
```

## Key Resources

| Topic | Link |
|-------|------|
| GTest primer | https://google.github.io/googletest/primer.html |
| GTest advanced | https://google.github.io/googletest/advanced.html |
| GMock for dummies | https://google.github.io/googletest/gmock_for_dummies.html |
| GMock cook book | https://google.github.io/googletest/gmock_cook_book.html |
| Custom matchers | https://google.github.io/googletest/gmock_cook_book.html#NewMatchers |

## Build & Run

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build -V           # all tests
./build/engine_tests --gtest_filter="Fixture*"        # just fixture tests
./build/engine_tests --gtest_filter="*Death*"         # just death tests
./build/engine_tests --gtest_list_tests               # list all test names
```

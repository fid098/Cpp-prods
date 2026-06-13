# Tutorial — GTest Trading Engine Suite

## Step 1 — TEST vs TEST_F

Use `TEST` for simple one-off tests. Use `TEST_F` (fixture) when multiple tests need the same setup.

```cpp
class OrderBookFixture : public ::testing::Test {
protected:
    void SetUp() override {
        book_.add_order(1, Side::BUY,  100, 174.90);
        book_.add_order(2, Side::SELL, 100, 175.10);
    }
    OrderBook book_;
};

TEST_F(OrderBookFixture, BestBidIsPriceSorted) {
    EXPECT_DOUBLE_EQ(book_.best_bid(), 174.90);
}
```

`SetUp()` runs before each `TEST_F`, `TearDown()` runs after. The fixture object is reconstructed for each test — they are isolated.

> **Docs:** https://google.github.io/googletest/primer.html#test-fixtures-using-the-same-data-configuration-for-multiple-tests-aka-test-fixtures

---

## Step 2 — EXPECT vs ASSERT

```cpp
EXPECT_EQ(a, b);   // continues the test on failure
ASSERT_EQ(a, b);   // aborts the test on failure (use when subsequent code would crash)
```

Use `ASSERT_*` when the rest of the test is meaningless if this one check fails:

```cpp
auto result = book_.find_order(999);
ASSERT_TRUE(result.has_value());   // without this, result->price would crash
EXPECT_DOUBLE_EQ(result->price, 175.0);
```

---

## Step 3 — Parameterized Tests

Avoid copy-pasting the same test logic with different inputs:

```cpp
struct OrderInput { int qty; double price; bool expected_valid; };

class OrderValidationTest : public ::testing::TestWithParam<OrderInput> {};

TEST_P(OrderValidationTest, ValidatesCorrectly) {
    auto [qty, price, expected] = GetParam();
    EXPECT_EQ(validate_order(qty, price), expected);
}

INSTANTIATE_TEST_SUITE_P(Boundary, OrderValidationTest, ::testing::Values(
    OrderInput{0,    175.0, false},  // zero qty
    OrderInput{1,    175.0, true},
    OrderInput{1000, 0.0,   false},  // zero price
    OrderInput{1000, 175.0, true}
));
```

> **Docs:** https://google.github.io/googletest/advanced.html#value-parameterized-tests

---

## Step 4 — Death Tests

```cpp
TEST(RiskLimitDeathTest, AssertFiresOnBreach) {
    EXPECT_DEATH(
        enforce_risk_limit(10'000'000),  // exceeds hard limit
        "risk limit breached"             // regex matched against stderr
    );
}
```

> **Docs:** https://google.github.io/googletest/advanced.html#death-tests

---

## Step 5 — GMock: Mock Objects

```cpp
class MockRiskChecker : public IRiskChecker {
public:
    MOCK_METHOD(bool, check_order, (int qty, double price), (const, override));
};

TEST(EngineTest, RejectsOrderWhenRiskCheckerReturnsFalse) {
    MockRiskChecker mock;
    EXPECT_CALL(mock, check_order(100, 175.0)).WillOnce(::testing::Return(false));

    TradingEngine engine{&mock};
    EXPECT_FALSE(engine.submit_order(100, 175.0));
}
```

> **Docs:** https://google.github.io/googletest/gmock_for_dummies.html

---

## Step 6 — EXPECT_THAT with Matchers

```cpp
#include <gmock/gmock.h>

TEST(ContainerTest, AllPricesPositive) {
    std::vector<double> prices = book_.all_ask_prices();
    EXPECT_THAT(prices, ::testing::Each(::testing::Gt(0.0)));
    EXPECT_THAT(prices, ::testing::IsSorted());
}
```

Key matchers: `Eq`, `Ne`, `Lt`, `Gt`, `Le`, `Ge`, `IsEmpty`, `SizeIs`, `Contains`, `Each`, `ElementsAre`, `IsSorted`.

> **Docs:** https://google.github.io/googletest/reference/matchers.html

---

## Step 7 — Typed Tests

Run the same test logic over multiple types (useful for template classes like `Stats<T>`):

```cpp
template <typename T>
class StatsTypedTest : public ::testing::Test {};
using NumericTypes = ::testing::Types<float, double, long double>;
TYPED_TEST_SUITE(StatsTypedTest, NumericTypes);

TYPED_TEST(StatsTypedTest, MeanOfSymmetricData) {
    Stats<TypeParam> s{{1.0, 2.0, 3.0}};
    EXPECT_NEAR(static_cast<double>(s.mean()), 2.0, 1e-5);
}
```

> **Docs:** https://google.github.io/googletest/advanced.html#typed-tests

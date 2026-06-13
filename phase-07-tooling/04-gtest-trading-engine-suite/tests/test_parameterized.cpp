#include <gtest/gtest.h>
#include "trading_engine.hpp"

// ---- Parameterized tests ---------------------------------------------------

struct OrderCase {
    int    qty;
    double price;
    bool   expected_valid;
};

class OrderValidationTest : public ::testing::TestWithParam<OrderCase> {};

TEST_P(OrderValidationTest, ValidatesCorrectly) {
    auto [qty, price, expected] = GetParam();
    EXPECT_EQ(validate_order(qty, price), expected);
}

INSTANTIATE_TEST_SUITE_P(
    BoundaryAndNormal,
    OrderValidationTest,
    ::testing::Values(
        OrderCase{0,    175.0, false},  // zero qty
        OrderCase{-1,   175.0, false},  // negative qty
        OrderCase{1,    0.0,   false},  // zero price
        OrderCase{1,   -1.0,   false},  // negative price
        OrderCase{1,    175.0, true},   // minimal valid
        OrderCase{1000, 175.0, true},   // normal
        OrderCase{1000000, 0.001, true} // large qty tiny price
    )
);

// ---- Typed test for floating point types ----------------------------------

template <typename T>
class BidAskTypedTest : public ::testing::Test {};
using FPTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(BidAskTypedTest, FPTypes);

TYPED_TEST(BidAskTypedTest, SpreadIsPositive) {
    double bid = 174.90;
    double ask = 175.10;
    EXPECT_GT(ask - bid, 0.0);
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "trading_engine.hpp"

using ::testing::Each;
using ::testing::Gt;
using ::testing::IsEmpty;
using ::testing::Not;
using ::testing::SizeIs;
using ::testing::IsSupersetOf;
using ::testing::Contains;
using ::testing::AllOf;

// ---- EXPECT_THAT with matchers --------------------------------------------

TEST(Matchers, AllAskPricesArePositive) {
    OrderBook book;
    book.add_order(1, Side::SELL, 100, 175.10);
    book.add_order(2, Side::SELL, 100, 175.50);
    book.add_order(3, Side::SELL, 100, 176.00);

    auto asks = book.all_ask_prices();
    EXPECT_THAT(asks, Not(IsEmpty()));
    EXPECT_THAT(asks, Each(Gt(0.0)));
    EXPECT_THAT(asks, SizeIs(3));
}

TEST(Matchers, AskPricesAreSorted) {
    OrderBook book;
    book.add_order(1, Side::SELL, 100, 176.00);
    book.add_order(2, Side::SELL, 100, 175.10);
    book.add_order(3, Side::SELL, 100, 175.50);

    auto asks = book.all_ask_prices();
    // std::map is sorted ascending, so ask prices should be in order
    EXPECT_THAT(asks, ::testing::IsSorted());
}

TEST(Matchers, AskPricesContainsExpectedValues) {
    OrderBook book;
    book.add_order(1, Side::SELL, 100, 175.10);
    book.add_order(2, Side::SELL, 100, 175.50);

    auto asks = book.all_ask_prices();
    EXPECT_THAT(asks, Contains(175.10));
    EXPECT_THAT(asks, Contains(175.50));
}

// ---- Custom matcher: IsValidOrder ----------------------------------------
// Demonstrates how to write a simple custom matcher using MATCHER_P2

MATCHER_P2(IsValidOrder, min_qty, max_qty,
           "order quantity is in [" + ::testing::PrintToString(min_qty)
           + ", " + ::testing::PrintToString(max_qty) + "]") {
    return arg > 0 && arg >= min_qty && arg <= max_qty;
}

TEST(CustomMatcher, IsValidOrderMatcherWorks) {
    EXPECT_THAT(100, IsValidOrder(1, 1000));
    EXPECT_THAT(0,   Not(IsValidOrder(1, 1000)));
    EXPECT_THAT(1001, Not(IsValidOrder(1, 1000)));
}

// ---- Death test -----------------------------------------------------------

TEST(DeathTest, EnforceRiskLimitAsserts) {
    // enforce_risk_limit calls assert() which crashes the process
    EXPECT_DEATH(enforce_risk_limit(10'000'000), ".*");
}

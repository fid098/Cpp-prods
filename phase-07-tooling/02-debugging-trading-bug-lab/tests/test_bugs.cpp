#include <gtest/gtest.h>
#include "pnl_tracker.hpp"
#include "order_manager.hpp"

// These tests FAIL until the bugs are fixed.

TEST(PnLTracker, AveragePriceCorrect) {
    double prices[] = {100.0, 102.0, 104.0};
    // Bug 1: off-by-one will read prices[3] (garbage), giving wrong average
    EXPECT_NEAR(compute_average_price(prices, 3), 102.0, 1e-9);
}

TEST(PnLTracker, TotalPnLNoOverflow) {
    Trade trades[] = {{1000000, 500.0}, {1000000, 500.0}};
    // Bug 2: int overflow gives negative result
    EXPECT_EQ(compute_total_pnl(trades, 2), 1'000'000'000LL);
}

TEST(OrderManager, FillExactQuantity) {
    OrderManager mgr;
    mgr.add_order(1, 100, 175.0);
    // Bug 3: fill_order(1, 100) returns false — should return true
    EXPECT_TRUE(mgr.fill_order(1, 100));
    EXPECT_TRUE(mgr.is_filled(1));
}

TEST(OrderManager, FillMoreThanEnough) {
    OrderManager mgr;
    mgr.add_order(2, 50, 200.0);
    EXPECT_TRUE(mgr.fill_order(2, 200));
}

TEST(OrderManager, DoNotFillInsufficientQuantity) {
    OrderManager mgr;
    mgr.add_order(3, 100, 175.0);
    EXPECT_FALSE(mgr.fill_order(3, 50));
}

TEST(OrderManager, GetOrderPriceThrowsOnUnknown) {
    OrderManager mgr;
    // Bug 4: crashes instead of throwing
    EXPECT_THROW(mgr.get_order_price(999), std::runtime_error);
}

#include <gtest/gtest.h>
#include "order_book.hpp"

TEST(LOB, AddBidUpdatesBestBid) {
    OrderBook b;
    b.add_order(1, true, 100.0, 50);
    b.add_order(2, true, 101.0, 50);
    EXPECT_DOUBLE_EQ(b.best_bid(), 101.0);
}

TEST(LOB, AddAskUpdatesBestAsk) {
    OrderBook b;
    b.add_order(1, false, 105.0, 100);
    b.add_order(2, false, 103.0, 100);
    EXPECT_DOUBLE_EQ(b.best_ask(), 103.0);
}

TEST(LOB, SpreadIsCorrect) {
    OrderBook b;
    b.add_order(1, true,  99.5, 10);
    b.add_order(2, false, 100.0, 10);
    EXPECT_NEAR(b.spread(), 0.5, 1e-9);
}

TEST(LOB, EmptyBookReturnsZero) {
    OrderBook b;
    EXPECT_DOUBLE_EQ(b.best_bid(), 0.0);
    EXPECT_DOUBLE_EQ(b.best_ask(), 0.0);
}

TEST(LOB, DuplicateIdReturnsFalse) {
    OrderBook b;
    EXPECT_TRUE(b.add_order(1, true, 100.0, 50));
    EXPECT_FALSE(b.add_order(1, true, 100.0, 50));
}

TEST(LOB, CancelRemovesOrder) {
    OrderBook b;
    b.add_order(1, true, 100.0, 50);
    EXPECT_TRUE(b.has_order(1));
    EXPECT_TRUE(b.cancel_order(1));
    EXPECT_FALSE(b.has_order(1));
}

TEST(LOB, CancelLastAtLevelRemovesLevel) {
    OrderBook b;
    b.add_order(1, true, 100.0, 50);
    b.cancel_order(1);
    EXPECT_EQ(b.bid_depth(), 0);
}

TEST(LOB, CancelMissingReturnsFalse) {
    OrderBook b;
    EXPECT_FALSE(b.cancel_order(999));
}

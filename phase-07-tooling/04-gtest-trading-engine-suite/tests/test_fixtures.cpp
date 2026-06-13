#include <gtest/gtest.h>
#include "trading_engine.hpp"

// ---- TEST_F fixture ---------------------------------------------------------

class OrderBookFixture : public ::testing::Test {
protected:
    void SetUp() override {
        book_.add_order(1, Side::BUY,  100, 174.90);
        book_.add_order(2, Side::BUY,  200, 174.50);
        book_.add_order(3, Side::SELL, 150, 175.10);
        book_.add_order(4, Side::SELL, 100, 175.50);
    }

    OrderBook book_;
};

TEST_F(OrderBookFixture, BestBidIsHighestBid) {
    EXPECT_DOUBLE_EQ(book_.best_bid(), 174.90);
}

TEST_F(OrderBookFixture, BestAskIsLowestAsk) {
    EXPECT_DOUBLE_EQ(book_.best_ask(), 175.10);
}

TEST_F(OrderBookFixture, OrderCountIsCorrect) {
    EXPECT_EQ(book_.order_count(), 4);
}

TEST_F(OrderBookFixture, CancelReducesCount) {
    book_.cancel_order(1);
    EXPECT_EQ(book_.order_count(), 3);
    EXPECT_FALSE(book_.has_order(1));
}

TEST_F(OrderBookFixture, CancelUnknownThrows) {
    EXPECT_THROW(book_.cancel_order(999), std::runtime_error);
}

TEST_F(OrderBookFixture, AddInvalidQtyThrows) {
    EXPECT_THROW(book_.add_order(10, Side::BUY, 0, 175.0), std::invalid_argument);
    EXPECT_THROW(book_.add_order(10, Side::BUY, -1, 175.0), std::invalid_argument);
}

TEST_F(OrderBookFixture, AddInvalidPriceThrows) {
    EXPECT_THROW(book_.add_order(10, Side::BUY, 100, 0.0),  std::invalid_argument);
    EXPECT_THROW(book_.add_order(10, Side::BUY, 100, -1.0), std::invalid_argument);
}

// ---- Empty book edge cases (no fixture needed) ----------------------------

TEST(OrderBookEmpty, BestBidThrowsWhenEmpty) {
    OrderBook b;
    EXPECT_THROW(b.best_bid(), std::runtime_error);
}

TEST(OrderBookEmpty, BestAskThrowsWhenEmpty) {
    OrderBook b;
    EXPECT_THROW(b.best_ask(), std::runtime_error);
}

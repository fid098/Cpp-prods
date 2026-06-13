#include <gtest/gtest.h>
#include "portfolio_cache.hpp"

TEST(PortfolioCache, AddAndFind) {
    PortfolioCache c;
    c.init(5);
    c.add("AAPL", 100, 150.0);
    const Position* p = c.find("AAPL");
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->quantity, 100);
    EXPECT_DOUBLE_EQ(p->avg_cost, 150.0);
    c.clear();
}

TEST(PortfolioCache, FindMissing) {
    PortfolioCache c;
    c.init(3);
    EXPECT_EQ(c.find("NVDA"), nullptr);
    c.clear();
}

TEST(PortfolioCache, TotalValue) {
    PortfolioCache c;
    c.init(5);
    c.add("AAPL", 100, 150.0);  // 15000
    c.add("MSFT", 200, 50.0);   // 10000
    EXPECT_DOUBLE_EQ(c.total_value(), 25000.0);
    c.clear();
}

TEST(PortfolioCache, ClearTwiceDoesNotCrash) {
    PortfolioCache c;
    c.init(5);
    c.add("X", 10, 5.0);
    c.clear();
    // After the bug is fixed, a second clear() should be safe (positions == nullptr)
    EXPECT_NO_FATAL_FAILURE(c.clear());
}

TEST(PortfolioCache, FullCacheRejectsFurther) {
    PortfolioCache c;
    c.init(2);
    EXPECT_TRUE(c.add("A", 1, 1.0));
    EXPECT_TRUE(c.add("B", 1, 1.0));
    EXPECT_FALSE(c.add("C", 1, 1.0));
    c.clear();
}

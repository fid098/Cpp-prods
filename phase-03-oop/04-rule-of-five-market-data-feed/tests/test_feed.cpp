#include <gtest/gtest.h>
#include "market_data_feed.hpp"

TEST(Feed, PushAndSize) {
    MarketDataFeed f{5};
    EXPECT_TRUE(f.push({100.0, 50, 1}));
    EXPECT_EQ(f.size(), 1);
}

TEST(Feed, FullRejectsPush) {
    MarketDataFeed f{2};
    f.push({1.0, 1, 0});
    f.push({2.0, 2, 0});
    EXPECT_FALSE(f.push({3.0, 3, 0}));
}

TEST(Feed, CopyIsDeep) {
    MarketDataFeed a{5};
    a.push({100.0, 10, 0});

    MarketDataFeed b = a;  // copy
    // Verify same value
    EXPECT_DOUBLE_EQ(b[0].price, 100.0);
    // Verify independence (different buffers)
    EXPECT_NE(static_cast<void*>(const_cast<Tick*>(&a[0])),
              static_cast<void*>(const_cast<Tick*>(&b[0])));
}

TEST(Feed, MoveEmptiesSource) {
    MarketDataFeed a{5};
    a.push({200.0, 20, 0});
    MarketDataFeed b = std::move(a);
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(b.size(), 1);
    EXPECT_DOUBLE_EQ(b[0].price, 200.0);
}

TEST(Feed, SelfAssignSafe) {
    MarketDataFeed a{5};
    a.push({50.0, 5, 0});
    a = a;  // self-assignment must not crash
    EXPECT_EQ(a.size(), 1);
}

TEST(Feed, CopyAssignmentDeep) {
    MarketDataFeed a{5}, b{3};
    a.push({77.0, 7, 0});
    b = a;
    EXPECT_DOUBLE_EQ(b[0].price, 77.0);
}

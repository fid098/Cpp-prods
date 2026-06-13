#include <gtest/gtest.h>
#include "feed_concepts.hpp"

TEST(Concepts, TickFeedSatisfiesDataFeed) {
    static_assert(DataFeed<TickFeed>);
    SUCCEED();
}

TEST(Concepts, MomentumStrategySatisfiesTradingStrategy) {
    static_assert(TradingStrategy<MomentumStrategy>);
    SUCCEED();
}

TEST(TickFeed, NextAdvancesPosition) {
    TickFeed f{"X", {1.0, 2.0, 3.0}};
    EXPECT_TRUE(f.next());
    EXPECT_DOUBLE_EQ(f.price(), 1.0);
    EXPECT_TRUE(f.next());
    EXPECT_DOUBLE_EQ(f.price(), 2.0);
    EXPECT_TRUE(f.next());
    EXPECT_FALSE(f.next());
}

TEST(TickFeed, SymbolIsCorrect) {
    TickFeed f{"MSFT", {100.0}};
    EXPECT_EQ(f.symbol(), "MSFT");
}

TEST(MomentumStrategy, ReturnsZeroBeforeLookback) {
    MomentumStrategy s{5, 0.02};
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(s.signal(100.0 + i, i), 0);
}

TEST(CountSignals, ZeroSignalsOnFlatSeries) {
    TickFeed feed{"F", {100, 100, 100, 100, 100, 100, 100, 100}};
    MomentumStrategy strat{3, 0.02};
    EXPECT_EQ(count_signals(feed, strat), 0);
}

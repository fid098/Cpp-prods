#include <gtest/gtest.h>
#include "factor_engine.hpp"

TEST(Factor, CompositeScoreEqualWeights) {
    Stock s{"X", 0.8, 0.6, 0.7};
    FactorWeights w{1.0/3, 1.0/3, 1.0/3};
    EXPECT_NEAR(composite_score(s, w), (0.8+0.6+0.7)/3, 1e-9);
}

TEST(Factor, RankUniverseDesc) {
    std::vector<Stock> stocks = {
        {"A", 0.2, 0.2, 0.2},
        {"B", 0.9, 0.9, 0.9},
    };
    FactorWeights w;
    auto ranked = rank_universe(stocks, w);
    EXPECT_EQ(ranked[0].symbol, "B");
}

TEST(Factor, TopNCorrectCount) {
    std::vector<Stock> stocks = {
        {"A", 0.5, 0.5, 0.5},
        {"B", 0.8, 0.8, 0.8},
        {"C", 0.3, 0.3, 0.3},
    };
    FactorWeights w;
    auto top = top_n(stocks, 2, w);
    ASSERT_EQ(top.size(), 2u);
    EXPECT_EQ(top[0].symbol, "B");
}

TEST(Factor, EqualWeight) {
    EXPECT_NEAR(equal_weight(4), 0.25, 1e-9);
    EXPECT_NEAR(equal_weight(1), 1.0,  1e-9);
    EXPECT_DOUBLE_EQ(equal_weight(0), 0.0);
}

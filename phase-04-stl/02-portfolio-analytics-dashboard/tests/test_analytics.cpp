#include <gtest/gtest.h>
#include "analytics.hpp"

TEST(Analytics, TotalValue) {
    std::vector<Holding> h = {{"A","X",100,10.0},{"B","Y",200,5.0}};
    EXPECT_DOUBLE_EQ(total_value(h), 2000.0);
}

TEST(Analytics, Weights) {
    std::vector<Holding> h = {{"A","X",100,100.0},{"B","Y",100,100.0}};
    auto w = weights(h);
    ASSERT_EQ(w.size(), 2u);
    EXPECT_NEAR(w[0], 0.5, 1e-9);
    EXPECT_NEAR(w[1], 0.5, 1e-9);
}

TEST(Analytics, HHI_EqualWeightIsLow) {
    std::vector<Holding> h = {{"A","X",100,100.0},{"B","Y",100,100.0},{"C","Z",100,100.0},{"D","W",100,100.0}};
    EXPECT_NEAR(herfindahl(h), 0.25, 1e-9);
}

TEST(Analytics, HHI_ConcentratedIsHigh) {
    // 99% in one holding
    std::vector<Holding> h = {{"A","X",990,100.0},{"B","Y",10,100.0}};
    EXPECT_GT(herfindahl(h), 0.9);
}

TEST(Analytics, SortByValue) {
    std::vector<Holding> h = {{"A","X",10,5.0},{"B","Y",100,10.0}};
    sort_by_value(h);
    EXPECT_EQ(h[0].symbol, "B");  // larger value first
}

TEST(Analytics, SectorExposure) {
    std::vector<Holding> h = {{"A","Tech",100,100.0},{"B","Tech",100,100.0},{"C","Finance",200,100.0}};
    auto exp = sector_exposure(h);
    EXPECT_NEAR(exp["Tech"],    0.5, 1e-9);
    EXPECT_NEAR(exp["Finance"], 0.5, 1e-9);
}

TEST(Analytics, LargestHolding) {
    std::vector<Holding> h = {{"A","X",10,1.0},{"B","Y",1000,100.0}};
    const Holding* l = largest_holding(h);
    ASSERT_NE(l, nullptr);
    EXPECT_EQ(l->symbol, "B");
}

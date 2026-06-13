#include <gtest/gtest.h>
#include "quantcore/price_filter.hpp"
#include "quantcore/spread.hpp"
#include <cmath>

// ---- PriceFilter -----------------------------------------------------------

TEST(PriceFilter, FirstUpdateReturnsPrice) {
    quantcore::PriceFilter f{0.5};
    EXPECT_DOUBLE_EQ(f.update(100.0), 100.0);
    EXPECT_TRUE(f.is_ready());
}

TEST(PriceFilter, SubsequentUpdateAppliesAlpha) {
    quantcore::PriceFilter f{0.5};
    f.update(100.0);
    double v = f.update(200.0);
    // EMA = 0.5*200 + 0.5*100 = 150
    EXPECT_DOUBLE_EQ(v, 150.0);
}

TEST(PriceFilter, Alpha1MeansNoSmoothing) {
    quantcore::PriceFilter f{1.0};
    f.update(50.0);
    EXPECT_DOUBLE_EQ(f.update(99.0), 99.0);
}

TEST(PriceFilter, InvalidAlphaThrows) {
    EXPECT_THROW(quantcore::PriceFilter{0.0},  std::invalid_argument);
    EXPECT_THROW(quantcore::PriceFilter{-0.1}, std::invalid_argument);
    EXPECT_THROW(quantcore::PriceFilter{1.1},  std::invalid_argument);
}

// ---- Spread ----------------------------------------------------------------

TEST(Spread, BasicComputation) {
    auto s = quantcore::compute_spread(174.90, 175.10);
    EXPECT_NEAR(s.absolute, 0.20,   1e-9);
    EXPECT_NEAR(s.mid,      175.00, 1e-9);
    EXPECT_NEAR(s.relative, 0.20 / 175.0, 1e-9);
}

TEST(Spread, ZeroBidThrows) {
    EXPECT_THROW(quantcore::compute_spread(0.0, 100.0), std::invalid_argument);
}

TEST(Spread, AskLessThanBidThrows) {
    EXPECT_THROW(quantcore::compute_spread(100.0, 99.0), std::invalid_argument);
}

TEST(Spread, TightSpread) {
    auto s = quantcore::compute_spread(100.00, 100.01);
    EXPECT_NEAR(s.absolute, 0.01, 1e-9);
    EXPECT_GT(s.relative,   0.0);
}

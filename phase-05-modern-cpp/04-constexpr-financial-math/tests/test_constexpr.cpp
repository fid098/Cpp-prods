#include <gtest/gtest.h>
#include <cmath>
#include "financial_math.hpp"

TEST(BpsPct, RoundTrip) {
    EXPECT_NEAR(pct_to_bps(bps_to_pct(150.0)), 150.0, 1e-9);
}

TEST(AnnualDaily, RoundTrip) {
    EXPECT_NEAR(daily_to_annual(annual_to_daily(0.05)), 0.05, 1e-9);
}

TEST(DiscountFactor, ZeroRateIsOne) {
    EXPECT_NEAR(discount_factor(0.0, 10.0), 1.0, 1e-6);
}

TEST(DiscountFactor, PositiveRateBelow1) {
    EXPECT_LT(discount_factor(0.05, 1.0), 1.0);
}

TEST(DiscountFactor, MatchesStdExp) {
    EXPECT_NEAR(discount_factor(0.05, 10.0), std::exp(-0.05 * 10.0), 1e-6);
}

TEST(FutureValue, ZeroRateUnchanged) {
    EXPECT_NEAR(future_value(1000.0, 0.0, 10.0), 1000.0, 1e-6);
}

TEST(FutureValue, PositiveRateGrows) {
    EXPECT_GT(future_value(1000.0, 0.05, 10.0), 1000.0);
}

TEST(ConstexprExp, MatchesStdExp) {
    for (double x : {-2.0, -1.0, 0.0, 0.5, 1.0, 2.0})
        EXPECT_NEAR(constexpr_exp(x), std::exp(x), 1e-8);
}

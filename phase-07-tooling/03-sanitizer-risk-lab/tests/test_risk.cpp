#include <gtest/gtest.h>
#include "var_calculator.hpp"
#include "portfolio_snapshot.hpp"
#include "position_sizer.hpp"
#include "risk_ratios.hpp"
#include <cmath>

// Tests for the FIXED versions of the buggy functions.

TEST(VaR, CorrectPercentile) {
    // Safe vector version (no bug)
    std::vector<double> returns = {-0.05, -0.03, -0.01, 0.01, 0.03, 0.05, -0.02, 0.02, -0.04, 0.04};
    double var = compute_var(returns, 0.95);
    EXPECT_GT(var, 0.0);  // VaR is a loss, expressed as positive
}

TEST(VaR, EmptyThrows) {
    EXPECT_THROW(compute_var({}, 0.95), std::invalid_argument);
}

TEST(VaR, BadConfidenceThrows) {
    EXPECT_THROW(compute_var({0.1}, 0.0),  std::invalid_argument);
    EXPECT_THROW(compute_var({0.1}, 1.0),  std::invalid_argument);
    EXPECT_THROW(compute_var({0.1}, -0.5), std::invalid_argument);
}

TEST(PositionSizer, SafeNoOverflow) {
    // size_position_safe must return 4,000,000,000 without overflow
    long long result = size_position_safe(2'000'000LL, 2000LL);
    EXPECT_EQ(result, 4'000'000'000LL);
}

TEST(RiskRatios, SharpeNormalCase) {
    double s = sharpe_ratio(0.10, 0.15);
    EXPECT_NEAR(s, 0.10 / 0.15, 1e-9);
}

TEST(RiskRatios, SharpeZeroVolReturnsZero) {
    // After the bug fix, sharpe_ratio(x, 0.0) should return 0.0, not inf
    EXPECT_DOUBLE_EQ(sharpe_ratio(0.1, 0.0), 0.0);
}

TEST(RiskRatios, SortinoNormalCase) {
    EXPECT_NEAR(sortino_ratio(0.08, 0.10), 0.8, 1e-9);
}

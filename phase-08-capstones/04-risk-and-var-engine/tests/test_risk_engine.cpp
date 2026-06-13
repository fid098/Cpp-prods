#include <gtest/gtest.h>
#include "risk_metrics.hpp"
#include <cmath>

TEST(Stats, MeanCorrect) {
    EXPECT_DOUBLE_EQ(risk::mean({1.0, 2.0, 3.0, 4.0, 5.0}), 3.0);
}

TEST(Stats, VarianceSampleCorrect) {
    // Variance of {2, 4, 4, 4, 5, 5, 7, 9} = 4.0 (sample)
    std::vector<double> v = {2, 4, 4, 4, 5, 5, 7, 9};
    EXPECT_NEAR(risk::variance(v, true), 4.0, 1e-9);
}

TEST(Stats, PearsonPerfectCorrelation) {
    std::vector<double> x = {1, 2, 3, 4, 5};
    std::vector<double> y = {2, 4, 6, 8, 10};
    EXPECT_NEAR(risk::pearson(x, y), 1.0, 1e-9);
}

TEST(Stats, PearsonNegativeCorrelation) {
    std::vector<double> x = {1, 2, 3, 4, 5};
    std::vector<double> y = {10, 8, 6, 4, 2};
    EXPECT_NEAR(risk::pearson(x, y), -1.0, 1e-9);
}

TEST(Stats, EmptyThrows) {
    EXPECT_THROW(risk::mean({}), std::invalid_argument);
    EXPECT_THROW(risk::historical_var({}, 0.95), std::invalid_argument);
}

TEST(VaR, HistoricalPositive) {
    std::vector<double> r;
    for (int i = -50; i <= 50; ++i) r.push_back(i * 0.001);
    EXPECT_GT(risk::historical_var(r, 0.95), 0.0);
}

TEST(VaR, ESGreaterThanOrEqualVaR) {
    std::vector<double> r;
    for (int i = -100; i <= 100; ++i) r.push_back(i * 0.001);
    double var = risk::historical_var(r, 0.95);
    double es  = risk::expected_shortfall(r, 0.95);
    EXPECT_GE(es, var);
}

TEST(VaR, ParametricConsistentWithHistorical) {
    // For large N normal sample, parametric and historical should be close
    std::mt19937 rng{42};
    std::normal_distribution<double> dist{0.0005, 0.01};
    std::vector<double> returns(10000);
    for (auto& x : returns) x = dist(rng);

    double hist  = risk::historical_var(returns, 0.95);
    double param = risk::parametric_var(risk::mean(returns), risk::stddev(returns), 0.95);
    EXPECT_NEAR(hist, param, 0.003);  // within 0.3%
}

TEST(Portfolio, MaxDrawdownZeroForMonotonicallyIncreasing) {
    std::vector<double> equity = {100, 101, 102, 103, 104};
    EXPECT_DOUBLE_EQ(risk::max_drawdown(equity), 0.0);
}

TEST(Portfolio, MaxDrawdownCorrect) {
    std::vector<double> equity = {100, 90, 80, 100, 95};
    // Peak=100 trough=80 → dd=20%
    EXPECT_NEAR(risk::max_drawdown(equity), 0.20, 1e-9);
}

TEST(Portfolio, HHIEqualWeightEqualsOneOverN) {
    std::vector<double> w = {0.25, 0.25, 0.25, 0.25};
    EXPECT_NEAR(risk::hhi(w), 0.25, 1e-9);
}

TEST(Portfolio, CorrelationMatrixDiagonalIsOne) {
    std::vector<double> x = {1, 2, 3, 4, 5};
    auto corr = risk::correlation_matrix({x, x});
    EXPECT_NEAR(corr[0][0], 1.0, 1e-9);
    EXPECT_NEAR(corr[1][1], 1.0, 1e-9);
}

TEST(Portfolio, BetaOfMarketIsOne) {
    std::vector<double> mkt = {0.01, -0.02, 0.03, -0.01, 0.02};
    EXPECT_NEAR(risk::portfolio_beta(mkt, mkt), 1.0, 1e-9);
}

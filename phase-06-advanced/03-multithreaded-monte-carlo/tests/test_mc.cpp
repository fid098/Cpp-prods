#include <gtest/gtest.h>
#include "monte_carlo.hpp"
#include <cmath>

// Black-Scholes analytical price for comparison
double bs_call(double S, double K, double r, double sigma, double T) {
    auto norm_cdf = [](double x) { return 0.5 * std::erfc(-x / std::sqrt(2.0)); };
    double d1 = (std::log(S/K) + (r + 0.5*sigma*sigma)*T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);
    return S*norm_cdf(d1) - K*std::exp(-r*T)*norm_cdf(d2);
}

TEST(MC, PriceConvergesNearBlackScholes) {
    MCParams p{100.0, 100.0, 0.05, 0.20, 1.0};
    double analytical = bs_call(p.S, p.K, p.r, p.sigma, p.T);

    auto r = mc_call_parallel(p, 500000, 4);

    // 500k paths should be within ~$0.10 of analytical
    EXPECT_NEAR(r.price, analytical, 0.5);
}

TEST(MC, SingleThreadedConsistent) {
    MCParams p{100.0, 100.0, 0.05, 0.20, 1.0};
    std::mt19937 rng{12345};
    double price = mc_call_single(p, 100000, rng);
    EXPECT_GT(price, 0.0);
    EXPECT_LT(price, p.S);
}

TEST(MC, ParallelVsSingleThreadClose) {
    MCParams p{100.0, 100.0, 0.05, 0.20, 1.0};
    auto parallel = mc_call_parallel(p, 200000, 2);

    std::mt19937 rng{42};
    double single = mc_call_single(p, 200000, rng);

    // Both should be within $1.0 of each other
    EXPECT_NEAR(parallel.price, single, 1.0);
}

TEST(MC, DeepITMCallCloseToForward) {
    // Deep in the money: spot=200, strike=100 → price ≈ spot - K*df ≈ 105
    MCParams p{200.0, 100.0, 0.05, 0.10, 1.0};
    auto r = mc_call_parallel(p, 200000, 2);
    double fwd_pv = 200.0 - 100.0 * std::exp(-0.05);
    EXPECT_NEAR(r.price, fwd_pv, 2.0);
}

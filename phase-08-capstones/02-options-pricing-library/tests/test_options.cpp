#include <gtest/gtest.h>
#include "bsm.hpp"
#include "binomial.hpp"
#include <cmath>

// Known BSM reference values (S=100, K=100, r=0.05, sigma=0.20, T=1)
// Call ≈ 10.4506, Put ≈ 5.5735  (from standard tables)

class BSMTest : public ::testing::Test {
protected:
    double S = 100, K = 100, r = 0.05, sigma = 0.20, T = 1.0;
    options::BSMResult res = options::bsm(S, K, r, sigma, T);
};

TEST_F(BSMTest, CallPriceKnownValue) {
    EXPECT_NEAR(res.call, 10.4506, 0.001);
}

TEST_F(BSMTest, PutPriceKnownValue) {
    EXPECT_NEAR(res.put, 5.5735, 0.001);
}

TEST_F(BSMTest, PutCallParity) {
    double lhs = res.call - res.put;
    double rhs = S - K * std::exp(-r * T);
    EXPECT_NEAR(lhs, rhs, 1e-8);
}

TEST_F(BSMTest, DeltaCallBetweenZeroAndOne) {
    EXPECT_GT(res.delta_call, 0.0);
    EXPECT_LT(res.delta_call, 1.0);
}

TEST_F(BSMTest, DeltaPutBetweenNegOneAndZero) {
    EXPECT_LT(res.delta_put, 0.0);
    EXPECT_GT(res.delta_put, -1.0);
}

TEST_F(BSMTest, DeltaSumEqualsOne) {
    EXPECT_NEAR(res.delta_call + std::abs(res.delta_put), 1.0, 1e-9);
}

TEST_F(BSMTest, GammaPositive) {
    EXPECT_GT(res.gamma, 0.0);
}

TEST_F(BSMTest, VegaPositive) {
    EXPECT_GT(res.vega, 0.0);
}

TEST_F(BSMTest, InvalidInputThrows) {
    EXPECT_THROW(options::bsm(0, K, r, sigma, T), std::invalid_argument);
    EXPECT_THROW(options::bsm(S, 0, r, sigma, T), std::invalid_argument);
    EXPECT_THROW(options::bsm(S, K, r, 0, T),     std::invalid_argument);
    EXPECT_THROW(options::bsm(S, K, r, sigma, 0), std::invalid_argument);
}

TEST(ImpliedVol, RecoversTrueSigma) {
    double S = 100, K = 100, r = 0.05, sigma = 0.25, T = 0.5;
    double market_price = options::bsm(S, K, r, sigma, T).call;
    double iv = options::implied_vol(S, K, r, T, market_price, true);
    EXPECT_NEAR(iv, sigma, 1e-6);
}

TEST(ImpliedVol, WorksForPut) {
    double S = 100, K = 105, r = 0.05, sigma = 0.30, T = 1.0;
    double market_price = options::bsm(S, K, r, sigma, T).put;
    double iv = options::implied_vol(S, K, r, T, market_price, false);
    EXPECT_NEAR(iv, sigma, 1e-6);
}

TEST(Binomial, ConvergesToBSM) {
    double S = 100, K = 100, r = 0.05, sigma = 0.20, T = 1.0;
    double bsm_call = options::bsm(S, K, r, sigma, T).call;
    double bin_call = options::binomial(S, K, r, sigma, T, 1000,
                        options::OptionType::CALL, options::ExerciseStyle::EUROPEAN);
    EXPECT_NEAR(bin_call, bsm_call, 0.01);
}

TEST(Binomial, AmericanPutGeEuropeanPut) {
    double S = 100, K = 110, r = 0.05, sigma = 0.25, T = 1.0;
    double euro = options::binomial(S, K, r, sigma, T, 200,
                    options::OptionType::PUT, options::ExerciseStyle::EUROPEAN);
    double amer = options::binomial(S, K, r, sigma, T, 200,
                    options::OptionType::PUT, options::ExerciseStyle::AMERICAN);
    EXPECT_GE(amer, euro);  // American >= European due to early exercise
}

TEST(Binomial, InvalidStepsThrows) {
    EXPECT_THROW(
        options::binomial(100, 100, 0.05, 0.2, 1.0, 0,
                          options::OptionType::CALL, options::ExerciseStyle::EUROPEAN),
        std::invalid_argument);
}

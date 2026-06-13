#include <gtest/gtest.h>
#include "equity.hpp"
#include "bond.hpp"
#include "european_option.hpp"

// ── Equity ────────────────────────────────────────────────────────────────────

TEST(Equity, PriceIsQtyTimesMarketPrice) {
    Equity eq{"AAPL", 100, 175.0};
    EXPECT_DOUBLE_EQ(eq.price(), 17500.0);
}

TEST(Equity, NameIsSymbol) {
    Equity eq{"MSFT", 50, 300.0};
    EXPECT_EQ(eq.name(), "MSFT");
}

TEST(Equity, SetPriceUpdates) {
    Equity eq{"TSLA", 10, 700.0};
    eq.set_price(750.0);
    EXPECT_DOUBLE_EQ(eq.price(), 7500.0);
}

// ── Bond ──────────────────────────────────────────────────────────────────────

TEST(Bond, ParBondAtParWhenCouponEqualsYTM) {
    // coupon_rate == ytm → PV == face
    Bond b{"T5Y", 1000.0, 0.05, 0.05, 5};
    EXPECT_NEAR(b.price(), 1000.0, 0.01);
}

TEST(Bond, PremiumBondWhenCouponAboveYTM) {
    // coupon > ytm → price > face
    Bond b{"CORP", 1000.0, 0.06, 0.04, 10};
    EXPECT_GT(b.price(), 1000.0);
}

TEST(Bond, DiscountBondWhenCouponBelowYTM) {
    Bond b{"JUNK", 1000.0, 0.03, 0.05, 10};
    EXPECT_LT(b.price(), 1000.0);
}

// ── EuropeanOption ────────────────────────────────────────────────────────────

TEST(Option, CallPricePositive) {
    EuropeanOption c{"AAPL", EuropeanOption::Type::CALL, 175.0, 180.0, 0.05, 0.20, 0.25};
    EXPECT_GT(c.price(), 0.0);
}

TEST(Option, PutPricePositive) {
    EuropeanOption p{"AAPL", EuropeanOption::Type::PUT, 175.0, 180.0, 0.05, 0.20, 0.25};
    EXPECT_GT(p.price(), 0.0);
}

TEST(Option, PutCallParity) {
    // C - P = S - K*exp(-rT)   (put-call parity)
    double S=175, K=180, r=0.05, T=0.25;
    EuropeanOption c{"X", EuropeanOption::Type::CALL, S, K, r, 0.20, T};
    EuropeanOption p{"X", EuropeanOption::Type::PUT,  S, K, r, 0.20, T};
    double lhs = c.price() - p.price();
    double rhs = S - K * std::exp(-r * T);
    EXPECT_NEAR(lhs, rhs, 0.01);
}

TEST(Option, NameIncludesType) {
    EuropeanOption c{"NVDA", EuropeanOption::Type::CALL, 500, 520, 0.05, 0.3, 0.5};
    EXPECT_NE(c.name().find("CALL"), std::string::npos);
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "platform.hpp"

// ============================================================
// BSM / Pricing
// ============================================================

class BSMFixture : public ::testing::Test {
protected:
    // ATM 1Y: S=100 K=100 r=0.05 sigma=0.20 T=1
    pricing::Greeks res = pricing::bsm(100, 100, 0.05, 0.20, 1.0, OptionType::CALL);
};

TEST_F(BSMFixture, CallPriceKnownValue) {
    EXPECT_NEAR(res.price, 10.4506, 0.01);
}

TEST_F(BSMFixture, DeltaBetweenZeroAndOne) {
    EXPECT_GT(res.delta, 0.0);
    EXPECT_LT(res.delta, 1.0);
}

TEST_F(BSMFixture, GammaAndVegaPositive) {
    EXPECT_GT(res.gamma, 0.0);
    EXPECT_GT(res.vega,  0.0);
}

TEST(Pricing, PutCallParity) {
    double S=100,K=100,r=0.05,sig=0.20,T=1;
    auto c = pricing::bsm(S,K,r,sig,T,OptionType::CALL);
    auto p = pricing::bsm(S,K,r,sig,T,OptionType::PUT);
    double lhs = c.price - p.price;
    double rhs = S - K*std::exp(-r*T);
    EXPECT_NEAR(lhs, rhs, 1e-6);
}

TEST(Pricing, InvalidParamsThrow) {
    EXPECT_THROW(pricing::bsm(0,100,0.05,0.2,1,OptionType::CALL), std::invalid_argument);
    EXPECT_THROW(pricing::bsm(100,0,0.05,0.2,1,OptionType::CALL), std::invalid_argument);
}

// ============================================================
// Order Book
// ============================================================

class BookFixture : public ::testing::Test {
protected:
    void SetUp() override {
        book_.add(Side::BUY,  100, 174.90);
        book_.add(Side::BUY,  200, 174.50);
        book_.add(Side::SELL, 150, 175.10);
        book_.add(Side::SELL, 100, 175.50);
    }
    OrderBook book_;
};

TEST_F(BookFixture, BestBidIsHighest) {
    EXPECT_DOUBLE_EQ(book_.best_bid(), 174.90);
}

TEST_F(BookFixture, BestAskIsLowest) {
    EXPECT_DOUBLE_EQ(book_.best_ask(), 175.10);
}

TEST_F(BookFixture, SpreadIsPositive) {
    EXPECT_GT(book_.spread(), 0.0);
}

// ============================================================
// Portfolio
// ============================================================

TEST(Portfolio, InitialCash) {
    PortfolioManager p{500000.0};
    EXPECT_DOUBLE_EQ(p.cash(), 500000.0);
}

TEST(Portfolio, FillUpdatesPosition) {
    PortfolioManager p{1000000.0};
    p.fill(FillEvent{"AAPL", 100, 175.0, 17.5});
    EXPECT_EQ(p.positions().at("AAPL").qty, 100);
    EXPECT_NEAR(p.positions().at("AAPL").avg_cost, 175.0, 1e-9);
}

TEST(Portfolio, UnrealisedPnLReflectsMarkToMarket) {
    PortfolioManager p{1000000.0};
    p.fill(FillEvent{"AAPL", 100, 170.0, 0});
    p.mark_to_market("AAPL", 175.0);
    EXPECT_NEAR(p.positions().at("AAPL").unrealised_pnl(), 500.0, 1e-6);
}

// ============================================================
// Risk Engine
// ============================================================

TEST(RiskEngine, VaRPositive) {
    RiskEngine r;
    std::vector<double> rets;
    for (int i = -100; i <= 100; ++i) rets.push_back(i * 0.001);
    r.add_returns(rets);
    EXPECT_GT(r.var95(), 0.0);
}

TEST(RiskEngine, ESGreaterThanVaR) {
    RiskEngine r;
    std::vector<double> rets;
    for (int i = -100; i <= 100; ++i) rets.push_back(i * 0.001);
    r.add_returns(rets);
    EXPECT_GE(r.es95(), r.var95());
}

// ============================================================
// Backtester
// ============================================================

TEST(Backtester, RunsOnSyntheticData) {
    Backtester bt;
    std::vector<BarEvent> bars;
    double p = 100.0;
    for (int i = 0; i < 100; ++i) {
        p += (i % 20 < 10) ? 0.5 : -0.3;
        bars.push_back({"SYM", p*0.99, p*1.01, p*0.98, p, 10000});
    }
    auto r = bt.run(bars);
    EXPECT_GE(r.max_drawdown, 0.0);
    EXPECT_LE(r.max_drawdown, 1.0);
}

// ============================================================
// Integration: price an option then record a trade
// ============================================================

TEST(Integration, PriceAndRecordOption) {
    auto g = pricing::bsm(175, 180, 0.05, 0.25, 0.5, OptionType::CALL);
    EXPECT_GT(g.price, 0.0);

    PortfolioManager port{100000.0};
    // Buy 10 option contracts at computed price
    port.fill(FillEvent{"AAPL_C180", 10, g.price, 0.1});
    EXPECT_EQ(port.positions().at("AAPL_C180").qty, 10);
}

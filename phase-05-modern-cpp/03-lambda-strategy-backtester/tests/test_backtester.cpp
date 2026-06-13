#include <gtest/gtest.h>
#include "backtester.hpp"

TEST(Backtest, AlwaysBuyNeverSell_NoTrades) {
    Strategy always_buy = [](const std::vector<double>&, int) { return 1; };
    std::vector<double> prices = {100, 101, 102, 103};
    auto r = backtest(prices, always_buy);
    // Opens a position but never closes it → 0 completed trades
    EXPECT_EQ(r.trades, 0);
}

TEST(Backtest, BuyThenSell_OneTrade) {
    std::vector<double> prices = {100, 100, 100, 100, 100};
    int call = 0;
    Strategy one_trade = [&call](const std::vector<double>&, int) -> int {
        ++call;
        if (call == 1) return 1;   // buy
        if (call == 3) return -1;  // sell
        return 0;
    };
    auto r = backtest(prices, one_trade);
    EXPECT_EQ(r.trades, 1);
}

TEST(Backtest, ProfitableRisingPrices) {
    std::vector<double> prices = {100, 101, 102, 103, 104};
    int i = 0;
    Strategy s = [&i](const std::vector<double>&, int) -> int {
        ++i;
        if (i == 1) return 1;   // buy at 101
        if (i == 4) return -1;  // sell at 104
        return 0;
    };
    auto r = backtest(prices, s, 10.0);
    EXPECT_NEAR(r.pnl, (104.0 - 101.0) * 10.0, 1e-9);
}

TEST(AndStrategy, OnlySignalsWhenBothAgree) {
    Strategy always1  = [](const std::vector<double>&, int) { return  1; };
    Strategy alwaysN1 = [](const std::vector<double>&, int) { return -1; };
    Strategy combined = and_strategy(always1, alwaysN1);
    std::vector<double> p = {1.0, 2.0};
    EXPECT_EQ(combined(p, 1), 0);  // disagree → 0
}

TEST(Invert, FlipsSignal) {
    Strategy s = [](const std::vector<double>&, int) { return 1; };
    auto inv = invert(s);
    std::vector<double> p = {1.0};
    EXPECT_EQ(inv(p, 0), -1);
}

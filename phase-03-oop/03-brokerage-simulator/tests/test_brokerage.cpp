#include <gtest/gtest.h>
#include "account.hpp"

TEST(CashAccount, BalanceDecreaseOnBuy) {
    CashAccount acc{10000.0};
    bool ok = acc.submit_order("AAPL", 10, 100.0, true); // 1000 + fee
    EXPECT_TRUE(ok);
    EXPECT_LT(acc.balance(), 10000.0);
}

TEST(CashAccount, BalanceIncreaseOnSell) {
    CashAccount acc{5000.0};
    acc.submit_order("AAPL", 10, 100.0, true);   // buy first
    double after_buy = acc.balance();
    acc.submit_order("AAPL", 10, 110.0, false);  // sell higher
    EXPECT_GT(acc.balance(), after_buy);
}

TEST(CashAccount, InsufficientFundsRejected) {
    CashAccount acc{100.0};
    bool ok = acc.submit_order("AAPL", 100, 150.0, true); // 15000 > 100
    EXPECT_FALSE(ok);
    EXPECT_DOUBLE_EQ(acc.balance(), 100.0); // unchanged
}

TEST(CashAccount, FeeMinimum) {
    CashAccount acc{10000.0};
    // 1 share @ $1 → notional=$1, 0.01% = $0.0001, but min is $1
    EXPECT_DOUBLE_EQ(acc.fee(1.0), 1.0);
}

TEST(CashAccount, FeePercentage) {
    CashAccount acc{10000.0};
    // 100000 * 0.0001 = $10
    EXPECT_DOUBLE_EQ(acc.fee(100000.0), 10.0);
}

TEST(MarginAccount, AcceptsLeveragedBuy) {
    MarginAccount acc{5000.0, 2.0};  // can buy up to 10000 notional
    bool ok = acc.submit_order("TSLA", 10, 700.0, true); // 7000 < 10000
    EXPECT_TRUE(ok);
}

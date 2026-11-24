#include <gtest/gtest.h>
#include <type_traits>
#include <typeinfo>   // std::bad_cast
#include "account.hpp"

// ── Compile-time guarantees about the hierarchy ─────────────────────────────
// static_assert fails the BUILD, not a test run, if the design regresses.
static_assert(std::is_abstract_v<Account>, "Account must be abstract");
static_assert(std::has_virtual_destructor_v<Account>, "polymorphic base needs virtual dtor");
static_assert(std::is_base_of_v<Account, CashAccount>);
static_assert(std::is_base_of_v<Account, MarginAccount>);

// ── CashAccount ─────────────────────────────────────────────────────────────
TEST(CashAccount, BalanceDecreaseOnBuy) {
    CashAccount acc{10000.0};
    bool ok = acc.submit_order("AAPL", 10, 100.0, true); // 1000 + fee
    EXPECT_TRUE(ok);
    EXPECT_LT(acc.balance(), 10000.0);
}

TEST(CashAccount, BuyDeductsNotionalPlusFee) {
    CashAccount acc{10000.0};
    acc.submit_order("AAPL", 10, 100.0, true);  // 1000 + $1 min fee
    EXPECT_DOUBLE_EQ(acc.balance(), 8999.0);
}

TEST(CashAccount, BalanceIncreaseOnSell) {
    CashAccount acc{5000.0};
    acc.submit_order("AAPL", 10, 100.0, true);   // buy first
    double after_buy = acc.balance();
    acc.submit_order("AAPL", 10, 110.0, false);  // sell higher
    EXPECT_GT(acc.balance(), after_buy);
}

TEST(CashAccount, SellCreditsNotionalMinusFee) {
    CashAccount acc{0.0};
    acc.submit_order("AAPL", 100, 200.0, false);  // 20000 - $2 fee
    EXPECT_DOUBLE_EQ(acc.balance(), 19998.0);
}

TEST(CashAccount, InsufficientFundsRejected) {
    CashAccount acc{100.0};
    bool ok = acc.submit_order("AAPL", 100, 150.0, true); // 15000 > 100
    EXPECT_FALSE(ok);
    EXPECT_DOUBLE_EQ(acc.balance(), 100.0); // unchanged
}

TEST(CashAccount, FeeCanTipBuyOverBalance) {
    CashAccount acc{1000.0};
    // notional = 1000 exactly, but + $1 fee = 1001 > 1000
    EXPECT_FALSE(acc.submit_order("AAPL", 10, 100.0, true));
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

TEST(CashAccount, InvalidOrdersRejected) {
    CashAccount acc{10000.0};
    EXPECT_FALSE(acc.submit_order("",     10, 100.0, true));   // no symbol
    EXPECT_FALSE(acc.submit_order("AAPL",  0, 100.0, true));   // zero qty
    EXPECT_FALSE(acc.submit_order("AAPL", -5, 100.0, true));   // negative qty
    EXPECT_FALSE(acc.submit_order("AAPL", 10,   0.0, true));   // zero price
    EXPECT_DOUBLE_EQ(acc.balance(), 10000.0);
}

// ── MarginAccount ───────────────────────────────────────────────────────────
TEST(MarginAccount, AcceptsLeveragedBuy) {
    MarginAccount acc{5000.0, 2.0};  // can buy up to 10000 notional
    bool ok = acc.submit_order("TSLA", 10, 700.0, true); // 7000 < 10000
    EXPECT_TRUE(ok);
}

TEST(MarginAccount, BorrowingMakesBalanceNegative) {
    MarginAccount acc{10000.0, 2.0};
    acc.submit_order("TSLA", 20, 700.0, true);  // 14000 + $0.70
    EXPECT_DOUBLE_EQ(acc.balance(), -4000.70);
    EXPECT_DOUBLE_EQ(acc.borrowed(), 4000.70);
}

TEST(MarginAccount, RejectsBuyBeyondLeverage) {
    MarginAccount acc{5000.0, 2.0};
    EXPECT_FALSE(acc.submit_order("TSLA", 11, 1000.0, true));  // 11000 > 10000
    EXPECT_DOUBLE_EQ(acc.balance(), 5000.0);
}

TEST(MarginAccount, AcceptsBuyExactlyAtLimit) {
    MarginAccount acc{5000.0, 2.0};
    // 9999.50 notional + $0.50 min fee = exactly 10000 buying power
    EXPECT_TRUE(acc.submit_order("X", 1, 9999.5, true));
    EXPECT_DOUBLE_EQ(acc.buying_power(), 0.0);
}

TEST(MarginAccount, LimitIsCumulativeAcrossOrders) {
    MarginAccount acc{5000.0, 2.0};
    EXPECT_TRUE(acc.submit_order("A", 6, 1000.0, true));   // uses 6000.50
    EXPECT_FALSE(acc.submit_order("B", 4, 1000.0, true));  // 4000.50 > 3999.50 left
}

TEST(MarginAccount, LeverageOneBehavesLikeCash) {
    MarginAccount acc{1000.0, 1.0};
    EXPECT_DOUBLE_EQ(acc.credit_limit(), 0.0);
    EXPECT_FALSE(acc.submit_order("A", 2, 600.0, true));
}

TEST(MarginAccount, FeeSchedule) {
    MarginAccount acc{0.0, 2.0};
    EXPECT_DOUBLE_EQ(acc.fee(1.0), 0.5);            // minimum
    EXPECT_DOUBLE_EQ(acc.fee(100000.0), 5.0);       // 0.005%
}

// ── Polymorphism ────────────────────────────────────────────────────────────
TEST(Polymorphism, FeeDispatchesThroughBaseReference) {
    CashAccount   cash{0.0};
    MarginAccount margin{0.0, 2.0};
    const Account& a = cash;
    const Account& b = margin;
    EXPECT_DOUBLE_EQ(a.fee(100000.0), 10.0);
    EXPECT_DOUBLE_EQ(b.fee(100000.0), 5.0);
}

TEST(Polymorphism, ProcessOrdersSetsStatusFeeAndBalance) {
    CashAccount acc{10000.0};
    std::vector<Order> orders = {
        {"AAPL", 10, 100.0, true},    // ok: 1001
        {"TSLA", 100, 700.0, true},   // rejected
    };
    process_orders(acc, orders);

    EXPECT_EQ(orders[0].status, Order::Status::FILLED);
    EXPECT_DOUBLE_EQ(orders[0].fee_paid, 1.0);
    EXPECT_DOUBLE_EQ(orders[0].balance_after, 8999.0);

    EXPECT_EQ(orders[1].status, Order::Status::REJECTED);
    EXPECT_DOUBLE_EQ(orders[1].fee_paid, 0.0);
    EXPECT_DOUBLE_EQ(orders[1].balance_after, 8999.0);
}

TEST(Polymorphism, SameOrdersDifferentOutcomes) {
    // The same order list is accepted by margin but rejected by cash —
    // process_orders() is identical; only the dynamic type differs.
    std::vector<Order> for_cash   = {{"TSLA", 20, 700.0, true}};
    std::vector<Order> for_margin = for_cash;

    CashAccount   cash{10000.0};
    MarginAccount margin{10000.0, 2.0};
    process_orders(cash, for_cash);
    process_orders(margin, for_margin);

    EXPECT_EQ(for_cash[0].status,   Order::Status::REJECTED);
    EXPECT_EQ(for_margin[0].status, Order::Status::FILLED);
}

TEST(Polymorphism, DynamicCastIdentifiesConcreteType) {
    CashAccount   cash{0.0};
    MarginAccount margin{0.0, 3.0};
    Account* a = &cash;
    Account* b = &margin;

    EXPECT_EQ(dynamic_cast<MarginAccount*>(a), nullptr);
    ASSERT_NE(dynamic_cast<MarginAccount*>(b), nullptr);
    EXPECT_DOUBLE_EQ(dynamic_cast<MarginAccount*>(b)->max_leverage(), 3.0);
}

TEST(Polymorphism, DynamicCastReferenceThrowsOnMismatch) {
    CashAccount cash{0.0};
    Account& a = cash;
    // Casting a REFERENCE can't return null, so failure throws std::bad_cast.
    EXPECT_THROW((void)dynamic_cast<MarginAccount&>(a), std::bad_cast);
}

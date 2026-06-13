#include <gtest/gtest.h>
#include "trade_validator.hpp"

// ── is_valid_symbol ───────────────────────────────────────────────────────────

TEST(Symbol, EmptyIsInvalid)         { EXPECT_FALSE(is_valid_symbol("")); }
TEST(Symbol, SingleCharIsValid)      { EXPECT_TRUE(is_valid_symbol("A")); }
TEST(Symbol, FiveCharsIsValid)       { EXPECT_TRUE(is_valid_symbol("GOOGL")); }
TEST(Symbol, SixCharsIsInvalid)      { EXPECT_FALSE(is_valid_symbol("TOOLNG")); }
TEST(Symbol, TypicalTicker)          { EXPECT_TRUE(is_valid_symbol("AAPL")); }

// ── is_valid_side ─────────────────────────────────────────────────────────────

TEST(Side, Buy)                      { EXPECT_TRUE(is_valid_side("BUY")); }
TEST(Side, Sell)                     { EXPECT_TRUE(is_valid_side("SELL")); }
TEST(Side, LowercaseInvalid)         { EXPECT_FALSE(is_valid_side("buy")); }
TEST(Side, RandomStringInvalid)      { EXPECT_FALSE(is_valid_side("HOLD")); }
TEST(Side, EmptyInvalid)             { EXPECT_FALSE(is_valid_side("")); }

// ── is_valid_qty (int overload) ───────────────────────────────────────────────

TEST(QtyInt, PositiveValid)          { EXPECT_TRUE(is_valid_qty(100)); }
TEST(QtyInt, ZeroInvalid)            { EXPECT_FALSE(is_valid_qty(0)); }
TEST(QtyInt, NegativeInvalid)        { EXPECT_FALSE(is_valid_qty(-1)); }

// ── is_valid_qty (double overload) ───────────────────────────────────────────

TEST(QtyDouble, PositiveValid)       { EXPECT_TRUE(is_valid_qty(50.0)); }
TEST(QtyDouble, ZeroInvalid)         { EXPECT_FALSE(is_valid_qty(0.0)); }
TEST(QtyDouble, NegativeInvalid)     { EXPECT_FALSE(is_valid_qty(-10.0)); }

// ── is_valid_type ─────────────────────────────────────────────────────────────

TEST(Type, Market)                   { EXPECT_TRUE(is_valid_type("MARKET")); }
TEST(Type, Limit)                    { EXPECT_TRUE(is_valid_type("LIMIT")); }
TEST(Type, InvalidType)              { EXPECT_FALSE(is_valid_type("STOP")); }
TEST(Type, Empty)                    { EXPECT_FALSE(is_valid_type("")); }

// ── is_valid_price ────────────────────────────────────────────────────────────

TEST(Price, LimitPositivePriceValid)    { EXPECT_TRUE(is_valid_price("LIMIT", 150.0)); }
TEST(Price, LimitZeroPriceInvalid)      { EXPECT_FALSE(is_valid_price("LIMIT", 0.0)); }
TEST(Price, LimitNegativePriceInvalid)  { EXPECT_FALSE(is_valid_price("LIMIT", -10.0)); }
TEST(Price, MarketZeroPriceValid)       { EXPECT_TRUE(is_valid_price("MARKET", 0.0)); }
TEST(Price, MarketAnyPriceValid)        { EXPECT_TRUE(is_valid_price("MARKET", 999.0)); }

// ── validate() ────────────────────────────────────────────────────────────────

TEST(Validate, GoodLimitOrder) {
    TradeTicket t{"AAPL", "BUY", 100, "LIMIT", 150.0};
    EXPECT_TRUE(validate(t).accepted);
}

TEST(Validate, GoodMarketOrder) {
    TradeTicket t{"MSFT", "SELL", 50, "MARKET", 0.0};
    EXPECT_TRUE(validate(t).accepted);
}

TEST(Validate, EmptySymbolRejected) {
    TradeTicket t{"", "BUY", 100, "LIMIT", 150.0};
    auto r = validate(t);
    EXPECT_FALSE(r.accepted);
    EXPECT_FALSE(r.reason.empty());
}

TEST(Validate, NegativeQtyRejected) {
    TradeTicket t{"TSLA", "BUY", -5, "LIMIT", 700.0};
    EXPECT_FALSE(validate(t).accepted);
}

TEST(Validate, LimitOrderZeroPriceRejected) {
    TradeTicket t{"NVDA", "BUY", 10, "LIMIT", 0.0};
    EXPECT_FALSE(validate(t).accepted);
}

TEST(Validate, InvalidSideRejected) {
    TradeTicket t{"AMZN", "HOLD", 20, "MARKET", 0.0};
    EXPECT_FALSE(validate(t).accepted);
}

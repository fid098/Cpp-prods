#include <gtest/gtest.h>
#include "pnl_calculator.hpp"

// ── compute_avg_cost ─────────────────────────────────────────────────────────

TEST(AvgCost, FirstBuy) {
    // Buying into a flat position (old_qty=0) should give exactly the new price.
    double avg = compute_avg_cost(0, 0.0, 100, 150.0);
    EXPECT_DOUBLE_EQ(avg, 150.0);
}

TEST(AvgCost, SecondBuyHigherPrice) {
    // 100 @ 150 then 50 @ 160 → (100*150 + 50*160) / 150 = 153.333...
    double avg = compute_avg_cost(100, 150.0, 50, 160.0);
    EXPECT_NEAR(avg, 153.333333, 1e-4);
}

TEST(AvgCost, SecondBuyLowerPrice) {
    // 200 @ 100 then 100 @ 70 → (200*100 + 100*70) / 300 = 90.0
    double avg = compute_avg_cost(200, 100.0, 100, 70.0);
    EXPECT_DOUBLE_EQ(avg, 90.0);
}

TEST(AvgCost, EqualPriceSameAvg) {
    double avg = compute_avg_cost(50, 200.0, 50, 200.0);
    EXPECT_DOUBLE_EQ(avg, 200.0);
}

// ── compute_realized_pnl ─────────────────────────────────────────────────────

TEST(RealizedPnl, ProfitOnSell) {
    Position pos{"AAPL", 100, 150.0, 0.0};
    double pnl = compute_realized_pnl(pos, 50, 175.0);
    // (175 - 150) * 50 = 1250
    EXPECT_DOUBLE_EQ(pnl, 1250.0);
}

TEST(RealizedPnl, LossOnSell) {
    Position pos{"MSFT", 100, 200.0, 0.0};
    double pnl = compute_realized_pnl(pos, 100, 180.0);
    // (180 - 200) * 100 = -2000
    EXPECT_DOUBLE_EQ(pnl, -2000.0);
}

TEST(RealizedPnl, BreakEven) {
    Position pos{"GOOG", 50, 100.0, 0.0};
    double pnl = compute_realized_pnl(pos, 50, 100.0);
    EXPECT_DOUBLE_EQ(pnl, 0.0);
}

TEST(RealizedPnl, PartialSell) {
    Position pos{"TSLA", 200, 250.0, 0.0};
    double pnl = compute_realized_pnl(pos, 25, 300.0);
    // (300 - 250) * 25 = 1250
    EXPECT_DOUBLE_EQ(pnl, 1250.0);
}

// ── compute_unrealized_pnl ───────────────────────────────────────────────────

TEST(UnrealizedPnl, PositiveGain) {
    Position pos{"AAPL", 100, 150.0, 170.0};
    double pnl = compute_unrealized_pnl(pos);
    // (170 - 150) * 100 = 2000
    EXPECT_DOUBLE_EQ(pnl, 2000.0);
}

TEST(UnrealizedPnl, UnrealizedLoss) {
    Position pos{"META", 50, 300.0, 250.0};
    double pnl = compute_unrealized_pnl(pos);
    // (250 - 300) * 50 = -2500
    EXPECT_DOUBLE_EQ(pnl, -2500.0);
}

TEST(UnrealizedPnl, NoPriceChangeIsZero) {
    Position pos{"AMD", 75, 120.0, 120.0};
    EXPECT_DOUBLE_EQ(compute_unrealized_pnl(pos), 0.0);
}

TEST(UnrealizedPnl, ZeroQuantityIsZero) {
    Position pos{"NVDA", 0, 500.0, 600.0};
    EXPECT_DOUBLE_EQ(compute_unrealized_pnl(pos), 0.0);
}

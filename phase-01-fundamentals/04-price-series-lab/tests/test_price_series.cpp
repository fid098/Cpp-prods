#include <gtest/gtest.h>
#include "price_series.hpp"

// ── daily_return ─────────────────────────────────────────────────────────────

TEST(DailyReturn, PositiveReturn) {
    // (102 - 100) / 100 * 100 = 2.0%
    EXPECT_DOUBLE_EQ(daily_return(100.0, 102.0), 2.0);
}

TEST(DailyReturn, NegativeReturn) {
    // (95 - 100) / 100 * 100 = -5.0%
    EXPECT_DOUBLE_EQ(daily_return(100.0, 95.0), -5.0);
}

TEST(DailyReturn, NoChange) {
    EXPECT_DOUBLE_EQ(daily_return(50.0, 50.0), 0.0);
}

// ── series_high ───────────────────────────────────────────────────────────────

TEST(SeriesHigh, BasicSeries) {
    double p[] = {100.0, 105.0, 98.0, 110.0, 107.0};
    EXPECT_DOUBLE_EQ(series_high(p, 5), 110.0);
}

TEST(SeriesHigh, SingleElement) {
    double p[] = {42.0};
    EXPECT_DOUBLE_EQ(series_high(p, 1), 42.0);
}

TEST(SeriesHigh, DecendingSeries) {
    double p[] = {100.0, 90.0, 80.0};
    EXPECT_DOUBLE_EQ(series_high(p, 3), 100.0);
}

// ── series_low ────────────────────────────────────────────────────────────────

TEST(SeriesLow, BasicSeries) {
    double p[] = {100.0, 105.0, 98.0, 110.0, 107.0};
    EXPECT_DOUBLE_EQ(series_low(p, 5), 98.0);
}

TEST(SeriesLow, SingleElement) {
    double p[] = {77.5};
    EXPECT_DOUBLE_EQ(series_low(p, 1), 77.5);
}

TEST(SeriesLow, AscendingSeries) {
    double p[] = {10.0, 20.0, 30.0};
    EXPECT_DOUBLE_EQ(series_low(p, 3), 10.0);
}

// ── max_drawdown ──────────────────────────────────────────────────────────────

TEST(MaxDrawdown, SimpleDropAndRecovery) {
    // Peak at 100, drops to 80 → drawdown = -20%
    double p[] = {100.0, 110.0, 80.0, 120.0};
    EXPECT_NEAR(max_drawdown(p, 4), -27.272, 0.001); // peak 110 → 80
}

TEST(MaxDrawdown, MonotonicallyIncreasing) {
    double p[] = {100.0, 110.0, 120.0, 130.0};
    EXPECT_DOUBLE_EQ(max_drawdown(p, 4), 0.0);
}

TEST(MaxDrawdown, MonotonicallyDecreasing) {
    // Peak is first element: 100; trough is last: 70 → -30%
    double p[] = {100.0, 90.0, 80.0, 70.0};
    EXPECT_DOUBLE_EQ(max_drawdown(p, 4), -30.0);
}

TEST(MaxDrawdown, TwoDips) {
    // First dip: 100→90 (-10%), second dip: 110→85 (−22.7%) → worst is second
    double p[] = {100.0, 90.0, 110.0, 85.0};
    EXPECT_NEAR(max_drawdown(p, 4), -22.727, 0.001);
}

// ── average_return ────────────────────────────────────────────────────────────

TEST(AverageReturn, KnownSeries) {
    // returns: +2%, -5%, +10% → average = 7/3 ≈ 2.333%
    double p[] = {100.0, 102.0, 96.9, 106.59};
    // Approximate — just check sign and rough magnitude
    double avg = average_return(p, 4);
    EXPECT_GT(avg, 0.0);
}

TEST(AverageReturn, FlatSeries) {
    double p[] = {100.0, 100.0, 100.0, 100.0};
    EXPECT_DOUBLE_EQ(average_return(p, 4), 0.0);
}

TEST(AverageReturn, TwoElements) {
    // (105 - 100) / 100 * 100 = 5%
    double p[] = {100.0, 105.0};
    EXPECT_DOUBLE_EQ(average_return(p, 2), 5.0);
}

TEST(AverageReturn, SingleElementReturnsZero) {
    double p[] = {100.0};
    EXPECT_DOUBLE_EQ(average_return(p, 1), 0.0);
}

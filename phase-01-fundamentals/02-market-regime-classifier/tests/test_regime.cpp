#include <gtest/gtest.h>
#include "regime_classifier.hpp"

// ── classify() ───────────────────────────────────────────────────────────────

TEST(Classify, StrongBull) {
    EXPECT_EQ(classify(1.5), Regime::BULL);
}

TEST(Classify, ExactBullBoundary) {
    EXPECT_EQ(classify(1.0), Regime::BULL);
}

TEST(Classify, StrongBear) {
    EXPECT_EQ(classify(-1.5), Regime::BEAR);
}

TEST(Classify, ExactBearBoundary) {
    EXPECT_EQ(classify(-1.0), Regime::BEAR);
}

TEST(Classify, PositiveFlat) {
    EXPECT_EQ(classify(0.5), Regime::FLAT);
}

TEST(Classify, NegativeFlat) {
    EXPECT_EQ(classify(-0.5), Regime::FLAT);
}

TEST(Classify, ZeroIsFlat) {
    EXPECT_EQ(classify(0.0), Regime::FLAT);
}

TEST(Classify, PositiveVolatile) {
    EXPECT_EQ(classify(2.5), Regime::VOLATILE);
}

TEST(Classify, NegativeVolatile) {
    EXPECT_EQ(classify(-3.1), Regime::VOLATILE);
}

TEST(Classify, ExactVolatileBoundaryAbove) {
    // Exactly 2.0% is NOT volatile (rule is strictly >2.0)
    EXPECT_EQ(classify(2.0), Regime::BULL);
}

TEST(Classify, ExactNegativeVolatileBoundary) {
    // Exactly -2.0% is NOT volatile
    EXPECT_EQ(classify(-2.0), Regime::BEAR);
}

TEST(Classify, LargePositiveReturn) {
    EXPECT_EQ(classify(10.0), Regime::VOLATILE);
}

TEST(Classify, LargeNegativeReturn) {
    EXPECT_EQ(classify(-8.5), Regime::VOLATILE);
}

// ── regime_to_string() ───────────────────────────────────────────────────────

TEST(RegimeToString, AllValues) {
    EXPECT_EQ(regime_to_string(Regime::BULL),     "BULL");
    EXPECT_EQ(regime_to_string(Regime::BEAR),     "BEAR");
    EXPECT_EQ(regime_to_string(Regime::VOLATILE), "VOLATILE");
    EXPECT_EQ(regime_to_string(Regime::FLAT),     "FLAT");
}

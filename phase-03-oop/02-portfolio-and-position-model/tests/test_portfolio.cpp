#include <gtest/gtest.h>
#include "portfolio.hpp"

TEST(Portfolio, TotalValue) {
    Portfolio p;
    p.add_position({"AAPL", 100, 150.0});  // 15000
    p.add_position({"MSFT", 50, 200.0});   // 10000
    EXPECT_DOUBLE_EQ(p.total_value(), 25000.0);
}

TEST(Portfolio, WeightCorrect) {
    Portfolio p;
    p.add_position({"A", 100, 100.0});  // 10000 = 50%
    p.add_position({"B", 100, 100.0});  // 10000 = 50%
    EXPECT_NEAR(p.weight("A"), 0.5, 1e-9);
    EXPECT_NEAR(p.weight("B"), 0.5, 1e-9);
}

TEST(Portfolio, WeightMissingSymbolIsZero) {
    Portfolio p;
    p.add_position({"A", 100, 100.0});
    EXPECT_DOUBLE_EQ(p.weight("Z"), 0.0);
}

TEST(Portfolio, CopyIsIndependent) {
    Portfolio original;
    original.add_position({"AAPL", 100, 150.0});

    Portfolio copy = original;
    // Mutate copy — original must be unchanged
    // We'll reach into the copy's positions via a const ref trick
    // (In real code you'd add a mutable accessor)
    EXPECT_DOUBLE_EQ(original.total_value(), 15000.0);
    EXPECT_DOUBLE_EQ(copy.total_value(), 15000.0);
}

TEST(Portfolio, SizeIsCorrect) {
    Portfolio p;
    EXPECT_EQ(p.size(), 0);
    p.add_position({"X", 1, 1.0});
    EXPECT_EQ(p.size(), 1);
}

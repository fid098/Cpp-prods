#include <gtest/gtest.h>
#include "risk_metrics.hpp"

TEST(UpdateByValue, DoesNotModifyCaller) {
    RiskMetrics m{0.0, 0.0, 0.0, 0.0, 0.0};
    update_by_value(m, 0.1, 1.5, 2.0);
    // caller should be unchanged
    EXPECT_DOUBLE_EQ(m.var,    0.0);
    EXPECT_DOUBLE_EQ(m.beta,   0.0);
    EXPECT_DOUBLE_EQ(m.sharpe, 0.0);
}

TEST(UpdateByPointer, ModifiesCaller) {
    RiskMetrics m{};
    update_by_pointer(&m, 0.05, 1.2, 1.8);
    EXPECT_DOUBLE_EQ(m.var,    0.05);
    EXPECT_DOUBLE_EQ(m.beta,   1.2);
    EXPECT_DOUBLE_EQ(m.sharpe, 1.8);
}

TEST(UpdateByPointer, NullptrSafe) {
    EXPECT_NO_FATAL_FAILURE(update_by_pointer(nullptr, 0.0, 0.0, 0.0));
}

TEST(UpdateByReference, ModifiesCaller) {
    RiskMetrics m{};
    update_by_reference(m, 0.03, 0.95, 2.1);
    EXPECT_DOUBLE_EQ(m.var,    0.03);
    EXPECT_DOUBLE_EQ(m.beta,   0.95);
    EXPECT_DOUBLE_EQ(m.sharpe, 2.1);
}

TEST(FormatMetrics, ContainsValues) {
    RiskMetrics m{0.05, 1.2, 1.8, 0.01, -0.15};
    std::string s = format_metrics(m);
    EXPECT_NE(s.find("0.05"), std::string::npos);
    EXPECT_NE(s.find("1.20"), std::string::npos);
}

#include <gtest/gtest.h>
#include "etf_analyzer.hpp"

std::vector<ETFHolding> sample() {
    return {{"A","I1","Tech",5.0},{"B","I2","Finance",3.0},{"C","I1","Tech",8.0}};
}

TEST(ETF, TopNReturnsCorrectCount) {
    auto top = top_n_holdings(sample(), 2);
    ASSERT_EQ(top.size(), 2u);
    EXPECT_EQ(top[0].symbol, "C");  // highest weight 8.0
}

TEST(ETF, SectorWeights) {
    auto sw = sector_weights(sample());
    EXPECT_NEAR(sw["Tech"], 13.0, 1e-9);
    EXPECT_NEAR(sw["Finance"], 3.0, 1e-9);
}

TEST(ETF, UniqueIssuers) {
    auto issuers = unique_issuers(sample());
    EXPECT_EQ(issuers.size(), 2u);  // I1 and I2
    EXPECT_NE(issuers.find("I1"), issuers.end());
    EXPECT_NE(issuers.find("I2"), issuers.end());
}

TEST(ETF, HoldingsBySectorGroupsCorrectly) {
    auto by_sector = holdings_by_sector(sample());
    EXPECT_EQ(by_sector.count("Tech"), 2u);
    EXPECT_EQ(by_sector.count("Finance"), 1u);
}

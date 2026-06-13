#include <gtest/gtest.h>
#include "pricing_graph.hpp"

TEST(SpotNode, ComputeReturnsPrice) {
    auto spot = std::make_shared<SpotNode>("AAPL", 175.0);
    EXPECT_DOUBLE_EQ(spot->compute(), 175.0);
}

TEST(SpotNode, UpdateChangesPrice) {
    auto spot = std::make_shared<SpotNode>("MSFT", 290.0);
    spot->update(300.0);
    EXPECT_DOUBLE_EQ(spot->compute(), 300.0);
}

TEST(ForwardNode, ForwardAboveSpot) {
    auto spot = std::make_shared<SpotNode>("X", 100.0);
    ForwardNode fwd{spot, 0.05, 1.0};  // F = 100 * exp(0.05) ≈ 105.13
    EXPECT_GT(fwd.compute(), 100.0);
    EXPECT_NEAR(fwd.compute(), 100.0 * std::exp(0.05), 0.01);
}

TEST(ForwardNode, UsesLatestSpot) {
    auto spot = std::make_shared<SpotNode>("X", 100.0);
    ForwardNode fwd{spot, 0.0, 1.0};  // F = S (rate=0)
    EXPECT_DOUBLE_EQ(fwd.compute(), 100.0);
    spot->update(110.0);
    EXPECT_DOUBLE_EQ(fwd.compute(), 110.0);
}

TEST(SpotObserver, SeesLivePrice) {
    auto spot = std::make_shared<SpotNode>("Y", 50.0);
    SpotObserver obs{spot};
    EXPECT_DOUBLE_EQ(obs.try_observe(), 50.0);
}

TEST(SpotObserver, ReturnsNegOneAfterDestroy) {
    SpotObserver obs{[]() {
        auto s = std::make_shared<SpotNode>("Z", 10.0);
        return std::weak_ptr<SpotNode>{s};
    }()};
    EXPECT_DOUBLE_EQ(obs.try_observe(), -1.0);
}

TEST(UniquePtr, MoveTransfersOwnership) {
    auto u = std::make_unique<SpotNode>("A", 1.0);
    auto u2 = std::move(u);
    EXPECT_EQ(u, nullptr);
    EXPECT_NE(u2, nullptr);
}

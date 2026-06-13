#include <gtest/gtest.h>
#include "tick_pipeline.hpp"

TickBatch make_batch(std::initializer_list<double> prices) {
    std::vector<Tick> ticks;
    for (double p : prices) ticks.push_back({p, 100, 0LL});
    return TickBatch{std::move(ticks), "TEST"};
}

TEST(ValidateStage, RemovesNonPositivePrices) {
    auto b = make_batch({100.0, -1.0, 50.0, 0.0, 75.0});
    b = validate_stage(std::move(b));
    EXPECT_EQ(b.size(), 3u);
}

TEST(ValidateStage, AllValidNoChange) {
    auto b = make_batch({1.0, 2.0, 3.0});
    b = validate_stage(std::move(b));
    EXPECT_EQ(b.size(), 3u);
}

TEST(ValidateStage, EmptyStaysEmpty) {
    TickBatch b{};
    b = validate_stage(std::move(b));
    EXPECT_TRUE(b.empty());
}

TEST(VWAP, KnownResult) {
    // 2 ticks: price=10 vol=100, price=20 vol=100 → VWAP = 15
    std::vector<Tick> ticks = {{10.0, 100, 0}, {20.0, 100, 0}};
    TickBatch b{std::move(ticks), "X"};
    EXPECT_DOUBLE_EQ(compute_vwap(b), 15.0);
}

TEST(VWAP, EmptyIsZero) {
    TickBatch b{};
    EXPECT_DOUBLE_EQ(compute_vwap(b), 0.0);
}

TEST(MoveSemantics, MoveEmptiesSource) {
    auto b = make_batch({1.0, 2.0});
    TickBatch b2 = std::move(b);
    EXPECT_TRUE(b.empty());
    EXPECT_EQ(b2.size(), 2u);
}

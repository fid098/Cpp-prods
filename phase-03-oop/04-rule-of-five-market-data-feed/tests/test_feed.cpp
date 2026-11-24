#include <gtest/gtest.h>
#include <type_traits>
#include <utility>
#include <vector>
#include "market_data_feed.hpp"

// ── Compile-time contract ───────────────────────────────────────────────────
static_assert(std::is_copy_constructible_v<MarketDataFeed>);
static_assert(std::is_copy_assignable_v<MarketDataFeed>);
static_assert(std::is_nothrow_move_constructible_v<MarketDataFeed>,
              "vector will copy instead of move if this isn't noexcept");
static_assert(std::is_nothrow_move_assignable_v<MarketDataFeed>);
static_assert(std::is_nothrow_destructible_v<MarketDataFeed>);
static_assert(!std::is_convertible_v<int, MarketDataFeed>, "ctor must be explicit");

// Every test starts with fresh counters and ends by checking for leaks.
class Feed : public ::testing::Test {
protected:
    void SetUp() override { FeedStats::reset(); }
    void TearDown() override {
        // All feeds in the test body are destroyed by now.
        EXPECT_EQ(FeedStats::live_buffers(), 0) << "memory leak or double free";
    }
};

// ── Basics ──────────────────────────────────────────────────────────────────
TEST_F(Feed, PushAndSize) {
    MarketDataFeed f{5};
    EXPECT_TRUE(f.push({100.0, 50, 1}));
    EXPECT_EQ(f.size(), 1);
}

TEST_F(Feed, FullRejectsPush) {
    MarketDataFeed f{2};
    f.push({1.0, 1, 0});
    f.push({2.0, 2, 0});
    EXPECT_FALSE(f.push({3.0, 3, 0}));
    EXPECT_EQ(f.size(), 2);
}

TEST_F(Feed, NegativeCapacityThrows) {
    EXPECT_THROW(MarketDataFeed{-1}, std::invalid_argument);
}

TEST_F(Feed, ZeroCapacityAllocatesNothing) {
    MarketDataFeed f{0};
    EXPECT_EQ(f.data(), nullptr);
    EXPECT_FALSE(f.push({1.0, 1, 0}));
    EXPECT_EQ(FeedStats::allocations, 0);
}

TEST_F(Feed, AtIsBoundsChecked) {
    MarketDataFeed f{5};
    f.push({1.0, 1, 0});
    EXPECT_DOUBLE_EQ(f.at(0).price, 1.0);
    EXPECT_THROW((void)f.at(1), std::out_of_range);   // capacity 5, but size 1
    EXPECT_THROW((void)f.at(-1), std::out_of_range);
}

// ── 1. Destructor ───────────────────────────────────────────────────────────
TEST_F(Feed, DestructorFreesBuffer) {
    {
        MarketDataFeed f{10};
        EXPECT_EQ(FeedStats::live_buffers(), 1);
    }
    EXPECT_EQ(FeedStats::frees, 1);
}

// ── 2. Copy constructor ─────────────────────────────────────────────────────
TEST_F(Feed, CopyIsDeep) {
    MarketDataFeed a{5};
    a.push({100.0, 10, 0});

    MarketDataFeed b = a;
    EXPECT_DOUBLE_EQ(b[0].price, 100.0);   // same values
    EXPECT_NE(a.data(), b.data());         // different buffers
    EXPECT_EQ(b.capacity(), 5);
    EXPECT_EQ(FeedStats::copies, 1);
}

TEST_F(Feed, CopyIsIndependent) {
    MarketDataFeed a{5};
    a.push({100.0, 10, 0});
    MarketDataFeed b = a;

    b[0].price = 999.0;
    b.push({2.0, 2, 0});
    EXPECT_DOUBLE_EQ(a[0].price, 100.0);   // original untouched
    EXPECT_EQ(a.size(), 1);
}

// ── 3. Copy assignment ──────────────────────────────────────────────────────
TEST_F(Feed, CopyAssignmentDeep) {
    MarketDataFeed a{5}, b{3};
    a.push({77.0, 7, 0});
    b = a;
    EXPECT_DOUBLE_EQ(b[0].price, 77.0);
    EXPECT_EQ(b.capacity(), 5);            // took the source's capacity
    EXPECT_NE(a.data(), b.data());
    EXPECT_EQ(FeedStats::frees, 1);        // b's old 3-slot buffer was freed
}

TEST_F(Feed, CopyAssignmentChains) {
    MarketDataFeed a{2}, b{2}, c{2};
    a.push({5.0, 5, 0});
    c = b = a;                             // needs operator= to return *this
    EXPECT_DOUBLE_EQ(c[0].price, 5.0);
}

TEST_F(Feed, SelfAssignSafe) {
    MarketDataFeed a{5};
    a.push({50.0, 5, 0});
    const Tick* before = a.data();
#if defined(__clang__)   // only Clang warns about self-assigning a class type
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#endif
    a = a;
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    EXPECT_EQ(a.size(), 1);
    EXPECT_DOUBLE_EQ(a[0].price, 50.0);
    EXPECT_EQ(a.data(), before);           // guard skipped the reallocation
}

// ── 4. Move constructor ─────────────────────────────────────────────────────
TEST_F(Feed, MoveEmptiesSource) {
    MarketDataFeed a{5};
    a.push({200.0, 20, 0});
    MarketDataFeed b = std::move(a);
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(b.size(), 1);
    EXPECT_DOUBLE_EQ(b[0].price, 200.0);
}

TEST_F(Feed, MoveStealsBufferWithoutAllocating) {
    MarketDataFeed a{5};
    const Tick* buf = a.data();
    MarketDataFeed b = std::move(a);
    EXPECT_EQ(b.data(), buf);              // same buffer, just a new owner
    EXPECT_EQ(a.data(), nullptr);
    EXPECT_EQ(a.capacity(), 0);
    EXPECT_EQ(FeedStats::allocations, 1);  // no new allocation
    EXPECT_EQ(FeedStats::copies, 0);
    EXPECT_EQ(FeedStats::moves, 1);
}

TEST_F(Feed, MovedFromIsReusable) {
    MarketDataFeed a{5};
    MarketDataFeed b = std::move(a);
    EXPECT_FALSE(a.push({1.0, 1, 0}));     // valid: just has no capacity
    a = MarketDataFeed{2};                 // can be given a new value
    EXPECT_TRUE(a.push({1.0, 1, 0}));
}

TEST_F(Feed, CopyOfMovedFromIsEmpty) {
    MarketDataFeed a{5};
    MarketDataFeed b = std::move(a);
    MarketDataFeed c = a;                  // copying an empty feed must work
    EXPECT_EQ(c.capacity(), 0);
    EXPECT_EQ(c.data(), nullptr);
}

// ── 5. Move assignment ──────────────────────────────────────────────────────
TEST_F(Feed, MoveAssignmentStealsAndFreesOld) {
    MarketDataFeed a{5}, b{3};
    a.push({42.0, 1, 0});
    const Tick* buf = a.data();
    b = std::move(a);
    EXPECT_EQ(b.data(), buf);
    EXPECT_DOUBLE_EQ(b[0].price, 42.0);
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(FeedStats::frees, 1);        // b's old buffer freed
}

TEST_F(Feed, SelfMoveAssignSafe) {
    MarketDataFeed a{5};
    a.push({9.0, 9, 0});
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-move"
#endif
    a = std::move(a);
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
    EXPECT_EQ(a.size(), 1);
    EXPECT_DOUBLE_EQ(a[0].price, 9.0);
}

// ── Interaction with the standard library ───────────────────────────────────
TEST_F(Feed, VectorGrowthMovesNotCopies) {
    std::vector<MarketDataFeed> feeds;
    for (int i = 0; i < 20; ++i) feeds.emplace_back(10);
    EXPECT_EQ(FeedStats::copies, 0);       // noexcept move → vector moves
    EXPECT_GT(FeedStats::moves, 0);        // reallocation did happen
    EXPECT_EQ(FeedStats::allocations, 20); // one buffer per feed, never more
}

TEST_F(Feed, ReturnByValueDoesNotCopy) {
    auto make = [] {
        MarketDataFeed f{4};
        f.push({1.0, 1, 0});
        return f;                          // moved (or elided), never copied
    };
    MarketDataFeed f = make();
    EXPECT_EQ(f.size(), 1);
    EXPECT_EQ(FeedStats::copies, 0);
}

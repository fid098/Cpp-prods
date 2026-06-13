#include <gtest/gtest.h>
#include "price_store.hpp"
#include <thread>
#include <vector>

TEST(PriceStore, RegisterAndSnapshot) {
    PriceStore store;
    ASSERT_TRUE(store.register_instrument("AAPL", 175.0));
    EXPECT_EQ(store.count(), 1);
    auto s = store.snapshot(0);
    EXPECT_TRUE(s.valid);
    EXPECT_EQ(s.symbol, "AAPL");
    EXPECT_GT(s.bid, 0.0);
    EXPECT_GT(s.ask, s.bid);
    EXPECT_DOUBLE_EQ(s.last, 175.0);
}

TEST(PriceStore, UpdateReflectedInSnapshot) {
    PriceStore store;
    store.register_instrument("MSFT", 400.0);
    store.update(0, 419.90, 420.10, 420.0);
    auto s = store.snapshot(0);
    EXPECT_NEAR(s.bid,  419.90, 1e-9);
    EXPECT_NEAR(s.ask,  420.10, 1e-9);
    EXPECT_NEAR(s.last, 420.0,  1e-9);
}

TEST(PriceStore, MaxCapacity) {
    PriceStore store;
    for (int i = 0; i < PriceStore::MAX_INSTRUMENTS; ++i)
        ASSERT_TRUE(store.register_instrument("S" + std::to_string(i), 100.0));
    EXPECT_FALSE(store.register_instrument("OVERFLOW", 100.0));
}

TEST(PriceStore, SpreadBpsPositive) {
    PriceStore store;
    store.register_instrument("TEST", 100.0);
    auto s = store.snapshot(0);
    EXPECT_GT(s.spread_bps(), 0.0);
}

TEST(PriceStore, ConcurrentReadsAndWrites) {
    PriceStore store;
    store.register_instrument("X", 100.0);

    std::atomic<bool> go{false};
    std::vector<std::thread> threads;

    // 2 writers
    for (int w = 0; w < 2; ++w) {
        threads.emplace_back([&]() {
            while (!go.load()) std::this_thread::yield();
            for (int i = 0; i < 10000; ++i)
                store.update(0, 99.0 + i*0.001, 101.0 + i*0.001, 100.0 + i*0.001);
        });
    }
    // 4 readers
    std::atomic<int> valid_reads{0};
    for (int r = 0; r < 4; ++r) {
        threads.emplace_back([&]() {
            while (!go.load()) std::this_thread::yield();
            for (int i = 0; i < 10000; ++i) {
                auto s = store.snapshot(0);
                if (s.valid) valid_reads.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    go.store(true);
    for (auto& t : threads) t.join();

    auto snap = store.snapshot(0);
    EXPECT_TRUE(snap.valid);
    EXPECT_GT(snap.bid, 0.0);
    EXPECT_GT(valid_reads.load(), 0);
}

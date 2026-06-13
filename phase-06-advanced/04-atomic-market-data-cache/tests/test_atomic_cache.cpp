#include <gtest/gtest.h>
#include "atomic_cache.hpp"
#include <thread>
#include <vector>

TEST(AtomicCache, UpdateAndReadBasic) {
    MarketDataCache cache;
    ASSERT_TRUE(cache.update("AAPL", 174.90, 175.10, 175.00));
    auto s = cache.read("AAPL");
    EXPECT_TRUE(s.valid);
    EXPECT_DOUBLE_EQ(s.bid,  174.90);
    EXPECT_DOUBLE_EQ(s.ask,  175.10);
    EXPECT_DOUBLE_EQ(s.last, 175.00);
}

TEST(AtomicCache, UnknownSymbolReturnsInvalid) {
    MarketDataCache cache;
    auto s = cache.read("UNKNOWN");
    EXPECT_FALSE(s.valid);
}

TEST(AtomicCache, UpdateCountIncrementsCorrectly) {
    MarketDataCache cache;
    cache.update("MSFT", 420.0, 420.5, 420.2);
    cache.update("MSFT", 421.0, 421.5, 421.2);
    cache.update("TSLA", 250.0, 250.5, 250.2);
    EXPECT_EQ(cache.update_count(), 3);
}

TEST(AtomicCache, MaxSymbolsRejected) {
    MarketDataCache cache;
    for (int i = 0; i < MarketDataCache::MAX_SYMBOLS; ++i) {
        ASSERT_TRUE(cache.update("SYM" + std::to_string(i), 1.0, 1.1, 1.05));
    }
    // One more should fail
    EXPECT_FALSE(cache.update("OVERFLOW", 1.0, 1.1, 1.05));
}

TEST(AtomicCache, ConcurrentReadersWriters) {
    MarketDataCache cache;
    // Pre-register symbols before concurrent access
    cache.update("AAPL", 0, 0, 0);
    cache.update("MSFT", 0, 0, 0);

    std::atomic<bool> go{false};
    std::vector<std::thread> threads;

    // 2 writers
    for (int w = 0; w < 2; ++w) {
        threads.emplace_back([&, w]() {
            while (!go.load()) std::this_thread::yield();
            for (int i = 0; i < 5000; ++i) {
                cache.update("AAPL", 174.0 + i*0.001, 175.0 + i*0.001, 174.5 + i*0.001);
                cache.update("MSFT", 420.0 + i*0.001, 421.0 + i*0.001, 420.5 + i*0.001);
            }
        });
    }

    // 4 readers
    std::atomic<int> valid_reads{0};
    for (int r = 0; r < 4; ++r) {
        threads.emplace_back([&]() {
            while (!go.load()) std::this_thread::yield();
            for (int i = 0; i < 5000; ++i) {
                auto s = cache.read("AAPL");
                if (s.valid) valid_reads.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    go.store(true);
    for (auto& t : threads) t.join();

    // After concurrent writes, the snapshot must still be valid and non-negative
    auto snap = cache.read("AAPL");
    EXPECT_TRUE(snap.valid);
    EXPECT_GT(snap.bid,  0.0);
    EXPECT_GT(snap.ask,  snap.bid);  // ask > bid
    EXPECT_GT(valid_reads.load(), 0);
}

TEST(AtomicCache, SizeReflectsRegistrations) {
    MarketDataCache cache;
    EXPECT_EQ(cache.size(), 0);
    cache.update("A", 1, 2, 1.5);
    EXPECT_EQ(cache.size(), 1);
    cache.update("B", 1, 2, 1.5);
    EXPECT_EQ(cache.size(), 2);
    cache.update("A", 2, 3, 2.5);  // update existing, size unchanged
    EXPECT_EQ(cache.size(), 2);
}

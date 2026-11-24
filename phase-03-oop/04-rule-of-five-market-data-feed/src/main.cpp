#include <iostream>
#include <utility>
#include <vector>
#include "market_data_feed.hpp"

static const char* yes_no(bool b) { return b ? "YES" : "NO"; }

static void print_stats(const char* label) {
    std::cout << "  [" << label << "] allocations=" << FeedStats::allocations
              << " frees=" << FeedStats::frees
              << " copies=" << FeedStats::copies
              << " moves=" << FeedStats::moves << "\n";
}

int main() {
    std::cout << "=== Rule of Five: Market Data Feed ===\n\n";

    {   // ← inner scope: every feed is destroyed at the closing brace
        MarketDataFeed feed1{1000};
        for (int i = 0; i < 1000; ++i)
            feed1.push({100.0 + i * 0.01, 100 + i, 1'000'000LL + i});
        std::cout << "feed1  capacity=" << feed1.capacity()
                  << "  ticks loaded: " << feed1.size() << "\n";

        // ── Copy constructor ────────────────────────────────────────────────
        MarketDataFeed feed2 = feed1;
        feed2[0].price = -1.0;   // mutate the copy…
        std::cout << "feed2 (copy of feed1)  capacity=" << feed2.capacity()
                  << "  independent buffer: "
                  << yes_no(feed2.data() != feed1.data() && feed1[0].price == 100.0)
                  << "\n";

        // ── Move constructor ────────────────────────────────────────────────
        const Tick* before = feed2.data();
        MarketDataFeed feed3 = std::move(feed2);
        std::cout << "feed3 (move from feed2)  capacity=" << feed3.capacity()
                  << "  feed2 is now empty: " << yes_no(feed2.empty())
                  << "  buffer stolen (same address): " << yes_no(feed3.data() == before)
                  << "\n";

        // ── Copy assignment (different sizes) ───────────────────────────────
        MarketDataFeed feed4{5};
        feed4 = feed1;
        std::cout << "feed4 = feed1  capacity 5 -> " << feed4.capacity() << "\n";

        // ── Move assignment ─────────────────────────────────────────────────
        feed4 = std::move(feed3);
        std::cout << "feed4 = std::move(feed3)  feed4[0].price=" << feed4[0].price
                  << "  feed3 empty: " << yes_no(feed3.empty()) << "\n";

        // ── Self-assignment ─────────────────────────────────────────────────
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-move"
#endif
        feed1 = feed1;
        feed1 = std::move(feed1);
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
        std::cout << "After self-assignment: feed1 capacity=" << feed1.capacity()
                  << "  size=" << feed1.size() << "\n";

        // ── Moved-from objects are still usable ─────────────────────────────
        feed2 = MarketDataFeed{3};   // assign a fresh value to a moved-from feed
        std::cout << "Reused moved-from feed2: capacity=" << feed2.capacity()
                  << " push ok: " << yes_no(feed2.push({1.0, 1, 1})) << "\n\n";

        print_stats("after demos");

        // ── Why noexcept matters: std::vector growth ────────────────────────
        // When a vector runs out of room it allocates a bigger array and
        // transfers the old elements. It MOVES them only if the move ctor is
        // noexcept; otherwise it COPIES (so a throw can't lose data).
        // (Measure a delta rather than reset(), so the final leak check still
        //  counts every buffer allocated above.)
        const int copies0 = FeedStats::copies, moves0 = FeedStats::moves;
        std::vector<MarketDataFeed> feeds;
        for (int i = 0; i < 5; ++i) feeds.emplace_back(100);
        std::cout << "\n5 x emplace_back into a vector (it reallocates as it grows):\n"
                  << "  copies=" << FeedStats::copies - copies0
                  << " moves=" << FeedStats::moves - moves0
                  << (FeedStats::copies == copies0
                          ? "  -> elements were MOVED, never copied, because the move ctor is noexcept\n\n"
                          : "  -> elements were COPIED: the move ctor isn't noexcept, so vector won't risk it\n\n");
    }   // ← all destructors run here

    print_stats("after scope");
    std::cout << "Leak check: live buffers = " << FeedStats::live_buffers()
              << (FeedStats::live_buffers() == 0 ? "  (no leaks)\n" : "  LEAK!\n");
    return 0;
}

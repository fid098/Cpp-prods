#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <vector>
#include "atomic_cache.hpp"

int main() {
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "=== Atomic Market Data Cache ===\n\n";

    MarketDataCache cache;

    // Register symbols before launching threads
    cache.update("AAPL", 0, 0, 0);
    cache.update("MSFT", 0, 0, 0);
    cache.update("TSLA", 0, 0, 0);

    // Publisher thread — simulate streaming tick data
    std::atomic<bool> running{true};
    std::thread publisher([&]() {
        double base[3] = {175.0, 420.0, 250.0};
        const char* syms[3] = {"AAPL", "MSFT", "TSLA"};
        for (int i = 0; running.load(std::memory_order_relaxed); ++i) {
            int s = i % 3;
            double spread = 0.02;
            double mid = base[s] + (i % 100) * 0.01;
            cache.update(syms[s], mid - spread, mid + spread, mid);
            std::this_thread::sleep_for(std::chrono::microseconds(50));
        }
    });

    // Four reader threads — each reads all symbols repeatedly
    std::vector<std::thread> readers;
    std::atomic<long long> total_reads{0};
    for (int t = 0; t < 4; ++t) {
        readers.emplace_back([&]() {
            const char* syms[3] = {"AAPL", "MSFT", "TSLA"};
            for (int i = 0; i < 2000; ++i) {
                for (auto sym : syms) {
                    auto snap = cache.read(sym);
                    if (snap.valid) total_reads.fetch_add(1, std::memory_order_relaxed);
                }
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
        });
    }

    for (auto& r : readers) r.join();
    running.store(false, std::memory_order_relaxed);
    publisher.join();

    std::cout << "Update count : " << cache.update_count() << "\n";
    std::cout << "Total reads  : " << total_reads.load() << "\n\n";

    std::cout << "Final snapshots:\n";
    for (auto sym : {"AAPL", "MSFT", "TSLA"}) {
        auto s = cache.read(sym);
        std::cout << "  " << sym
                  << "  bid=" << s.bid
                  << "  ask=" << s.ask
                  << "  last=" << s.last
                  << "  valid=" << std::boolalpha << s.valid << "\n";
    }

    return 0;
}

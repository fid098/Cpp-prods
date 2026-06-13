#include <iostream>
#include <utility>
#include "market_data_feed.hpp"

int main() {
    std::cout << "=== Rule of Five: Market Data Feed ===\n\n";

    MarketDataFeed feed1{10};
    for (int i = 0; i < 10; ++i)
        feed1.push({100.0 + i, 100 + i, 1000000LL + i});
    std::cout << "feed1 size=" << feed1.size() << " capacity=" << feed1.capacity() << "\n";

    // Copy constructor
    MarketDataFeed feed2 = feed1;
    std::cout << "feed2 (copy) size=" << feed2.size() << "\n";
    std::cout << "Independent? " << (feed2[0].price == feed1[0].price ? "shares value" : "") << "\n";

    // Move constructor
    MarketDataFeed feed3 = std::move(feed2);
    std::cout << "feed3 (moved) size=" << feed3.size() << "\n";
    std::cout << "feed2 empty after move: " << (feed2.empty() ? "YES" : "NO") << "\n";

    return 0;
}

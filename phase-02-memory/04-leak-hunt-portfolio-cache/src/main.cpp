#include <iostream>
#include <iomanip>
#include "portfolio_cache.hpp"

int main() {
    std::cout << "=== Portfolio Cache ===\n\n";

    PortfolioCache cache;
    cache.init(10);

    cache.add("AAPL", 100, 150.0);
    cache.add("MSFT", 200, 290.0);
    cache.add("TSLA",  50, 700.0);

    std::cout << "Loaded " << cache.count << " positions:\n";
    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < cache.count; ++i) {
        const Position& p = cache.positions[i];
        std::cout << "  " << p.symbol
                  << "  qty=" << p.quantity
                  << "  cost=" << p.avg_cost << "\n";
    }
    std::cout << "\nTotal value: $" << cache.total_value() << "\n\n";

    cache.refresh_metadata();  // BUG 1 is inside here

    cache.clear();   // first clear
    cache.clear();   // BUG 2: double-free — fix in portfolio_cache.hpp

    return 0;
}

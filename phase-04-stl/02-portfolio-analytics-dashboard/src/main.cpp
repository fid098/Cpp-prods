#include <iostream>
#include <iomanip>
#include "analytics.hpp"

int main() {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "=== Portfolio Analytics Dashboard ===\n\n";

    std::vector<Holding> portfolio = {
        {"AAPL", "Tech",    100, 175.0},
        {"MSFT", "Tech",    200, 290.0},
        {"JPM",  "Finance", 150, 180.0},
        {"GS",   "Finance",  50, 350.0},
    };

    sort_by_value(portfolio);

    double total = total_value(portfolio);
    auto   w     = weights(portfolio);

    std::cout << "Symbol  Sector    Qty   Price    Value     Weight\n";
    for (std::size_t i = 0; i < portfolio.size(); ++i) {
        const auto& h = portfolio[i];
        std::cout << std::left << std::setw(8) << h.symbol
                  << std::setw(10) << h.sector
                  << std::right << std::setw(5) << h.qty
                  << std::setw(8) << h.price
                  << std::setw(10) << h.value()
                  << std::setw(8) << (w.empty() ? 0.0 : w[i] * 100) << "%\n";
    }
    std::cout << "\nTotal value: " << total << "\n";
    std::cout << "HHI (concentration): " << herfindahl(portfolio) << "\n\n";

    std::cout << "Sector exposure:\n";
    for (const auto& [sector, weight] : sector_exposure(portfolio))
        std::cout << "  " << sector << ": " << (weight * 100) << "%\n";

    return 0;
}

#include <iostream>
#include <iomanip>
#include "etf_analyzer.hpp"

int main() {
    std::cout << "=== ETF Holdings Analyzer ===\n\n";

    std::vector<ETFHolding> holdings = {
        {"AAPL", "Apple",       "Tech",     7.0},
        {"MSFT", "Microsoft",   "Tech",     6.5},
        {"AMZN", "Amazon",      "Consumer", 3.5},
        {"JPM",  "JPMorgan",    "Finance",  2.8},
        {"NVDA", "NVIDIA",      "Tech",     2.5},
        {"BRK",  "Berkshire",   "Finance",  2.0},
        {"TSLA", "Tesla",       "Consumer", 1.5},
    };

    std::cout << "Top 3 holdings:\n";
    for (const auto& h : top_n_holdings(holdings, 3))
        std::cout << "  " << h.symbol << "  " << h.weight << "%\n";

    std::cout << "\nSector weights:\n";
    for (const auto& [sector, w] : sector_weights(holdings))
        std::cout << "  " << std::setw(10) << std::left << sector << "  " << std::fixed << std::setprecision(1) << w << "%\n";

    std::cout << "\nUnique issuers (" << unique_issuers(holdings).size() << "):\n";
    for (const auto& issuer : unique_issuers(holdings))
        std::cout << "  " << issuer << "\n";

    return 0;
}

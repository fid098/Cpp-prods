#include <iostream>
#include <iomanip>
#include "factor_engine.hpp"

int main() {
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "=== Factor Ranking Engine ===\n\n";

    std::vector<Stock> universe = {
        {"AAPL", 0.75, 0.70, 0.80},
        {"MSFT", 0.65, 0.55, 0.90},
        {"NVDA", 0.90, 0.60, 0.85},
        {"TSLA", 0.80, 0.30, 0.60},
        {"JPM",  0.50, 0.85, 0.75},
        {"GS",   0.55, 0.80, 0.70},
    };

    FactorWeights w{0.4, 0.3, 0.3};

    auto ranked = rank_universe(universe, w);

    std::cout << "Ranked universe:\n";
    std::cout << std::left;
    int rank = 1;
    for (const auto& s : ranked) {
        std::cout << "  " << rank++ << ". " << std::setw(6) << s.symbol
                  << "  mom=" << s.momentum
                  << "  val=" << s.value
                  << "  qual=" << s.quality
                  << "  score=" << composite_score(s, w) << "\n";
    }

    std::cout << "\nTop 3 equal-weight portfolio (" << std::setprecision(1) << equal_weight(3)*100 << "% each):\n";
    for (const auto& s : top_n(universe, 3, w))
        std::cout << "  " << s.symbol << "\n";

    return 0;
}

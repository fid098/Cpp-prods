#include <array>
#include <iostream>
#include <iomanip>
#include "price_series.hpp"

int main() {
    std::array<double, 7> prices = {100.0, 102.5, 101.0, 105.0, 98.0, 107.0, 103.5};

    const double* p = prices.data();
    std::size_t   n = prices.size();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "=== Price Series Analysis ===\n";
    std::cout << "Prices:";
    for (double px : prices) std::cout << "  " << px;
    std::cout << "\n\n";

    std::cout << "Daily returns (%):\n";
    for (std::size_t i = 1; i < n; ++i) {
        double r = daily_return(p[i - 1], p[i]);
        std::cout << "  Day " << i << "→" << (i + 1) << ":  "
                  << (r >= 0 ? "+" : "") << r << "%\n";
    }

    std::cout << "\nStatistics:\n";
    std::cout << "  High:           " << series_high(p, n)    << "\n";
    std::cout << "  Low:            " << series_low(p, n)     << "\n";
    std::cout << "  Max Drawdown:   " << max_drawdown(p, n)   << "%\n";
    std::cout << "  Average Return: " << average_return(p, n) << "%\n";

    return 0;
}

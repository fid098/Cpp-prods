#include <iostream>
#include <iomanip>
#include <vector>
#include "quantcore/price_filter.hpp"
#include "quantcore/spread.hpp"

int main() {
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "=== QuantCore Library Demo ===\n\n";

    // EMA price filter
    quantcore::PriceFilter ema{0.2};
    std::vector<double> prices = {100.0, 102.5, 101.0, 105.0, 103.5, 106.0};

    std::cout << "EMA (alpha=0.2):\n";
    for (double p : prices) {
        std::cout << "  raw=" << p << "  ema=" << ema.update(p) << "\n";
    }

    // Spread calculator
    std::cout << "\nSpread for bid=174.90 ask=175.10:\n";
    auto s = quantcore::compute_spread(174.90, 175.10);
    std::cout << "  absolute=" << s.absolute
              << "  relative=" << s.relative * 100.0 << "%"
              << "  mid=" << s.mid << "\n";

    return 0;
}

#include <iostream>
#include <iomanip>
#include "backtester.hpp"

int main() {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "=== Lambda Strategy Backtester ===\n\n";

    std::vector<double> prices = {
        100, 102, 101, 105, 103, 107, 110, 108, 112, 109,
        115, 113, 117, 120, 118, 122, 119, 124, 121, 125
    };

    auto momentum = momentum_strategy(5, 0.03);
    auto mean_rev = mean_reversion_strategy(5);
    auto combined = and_strategy(momentum, mean_rev);

    auto run = [&](const std::string& name, const Strategy& s) {
        auto r = backtest(prices, s);
        std::cout << name << ":  PnL=" << r.pnl << "  trades=" << r.trades << "\n";
    };

    run("Momentum         ", momentum);
    run("Mean Reversion   ", mean_rev);
    run("Combined (AND)   ", combined);
    run("Inverted Momentum", invert(momentum));

    return 0;
}

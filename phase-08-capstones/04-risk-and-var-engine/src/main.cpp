#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include "risk_metrics.hpp"

int main() {
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "=== Risk & VaR Engine ===\n\n";

    // Generate synthetic daily returns for 3 assets (252 days)
    std::mt19937 rng{42};
    auto gen_returns = [&](double mu, double sigma, int n) {
        std::normal_distribution<double> dist{mu / 252.0, sigma / std::sqrt(252.0)};
        std::vector<double> v(n);
        for (auto& x : v) x = dist(rng);
        return v;
    };

    auto aapl = gen_returns(0.15, 0.25, 252);
    auto msft = gen_returns(0.12, 0.22, 252);
    auto tsla = gen_returns(0.20, 0.50, 252);
    auto mkt  = gen_returns(0.10, 0.18, 252);

    // Portfolio: 40% AAPL, 40% MSFT, 20% TSLA
    std::vector<double> weights = {0.4, 0.4, 0.2};
    std::vector<double> port_returns(252);
    for (int i = 0; i < 252; ++i)
        port_returns[i] = weights[0]*aapl[i] + weights[1]*msft[i] + weights[2]*tsla[i];

    // Build equity curve
    std::vector<double> equity(252);
    equity[0] = 1'000'000.0;
    for (int i = 1; i < 252; ++i)
        equity[i] = equity[i-1] * (1.0 + port_returns[i]);

    // VaR
    std::cout << "=== VaR (95%, 1-day) ===\n";
    std::cout << "  Historical:   $" << risk::historical_var(port_returns, 0.95) * equity[0] << "\n";
    std::cout << "  Parametric:   $" << risk::parametric_var(
                    risk::mean(port_returns), risk::stddev(port_returns), 0.95) * equity[0] << "\n";
    std::cout << "  Monte Carlo:  $" << risk::mc_var(
                    risk::mean(port_returns), risk::stddev(port_returns), 0.95) * equity[0] << "\n";
    std::cout << "  E[S] (CVaR):  $" << risk::expected_shortfall(port_returns, 0.95) * equity[0] << "\n\n";

    // Portfolio statistics
    std::cout << "=== Portfolio Statistics ===\n";
    std::cout << "  Annualised Sharpe: " << risk::annualised_sharpe(port_returns) << "\n";
    std::cout << "  Max Drawdown:      " << risk::max_drawdown(equity) * 100.0 << "%\n";
    std::cout << "  Portfolio Beta:    " << risk::portfolio_beta(port_returns, mkt) << "\n";
    std::cout << "  HHI:               " << risk::hhi(weights) << "  (1/n=" << 1.0/3.0 << ")\n\n";

    // Correlation matrix
    auto corr = risk::correlation_matrix({aapl, msft, tsla});
    std::cout << "=== Correlation Matrix (AAPL, MSFT, TSLA) ===\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << "  ";
        for (int j = 0; j < 3; ++j)
            std::cout << std::setw(8) << corr[i][j] << " ";
        std::cout << "\n";
    }

    return 0;
}

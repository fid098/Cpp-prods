#include <iostream>
#include <iomanip>
#include "bsm.hpp"
#include "binomial.hpp"

int main() {
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "=== Options Pricing Library ===\n\n";

    double S = 100.0, K = 100.0, r = 0.05, sigma = 0.20, T = 1.0;

    // Black-Scholes-Merton
    auto res = options::bsm(S, K, r, sigma, T);
    std::cout << "--- Black-Scholes-Merton (ATM, 1Y) ---\n";
    std::cout << "Call price:   " << res.call       << "\n";
    std::cout << "Put  price:   " << res.put        << "\n";
    std::cout << "Delta (call): " << res.delta_call << "\n";
    std::cout << "Gamma:        " << res.gamma      << "\n";
    std::cout << "Vega:         " << res.vega       << "\n";
    std::cout << "Theta (call): " << res.theta_call << "\n";

    // Put-call parity check
    double parity_lhs = res.call - res.put;
    double parity_rhs = S - K * std::exp(-r * T);
    std::cout << "\nPut-call parity: C-P=" << parity_lhs
              << "  S-Ke^(-rT)=" << parity_rhs
              << "  diff=" << (parity_lhs - parity_rhs) << "\n";

    // Binomial tree comparison
    std::cout << "\n--- Binomial CRR (500 steps) vs BSM ---\n";
    using namespace options;
    double bc = binomial(S, K, r, sigma, T, 500, OptionType::CALL, ExerciseStyle::EUROPEAN);
    double bp = binomial(S, K, r, sigma, T, 500, OptionType::PUT,  ExerciseStyle::EUROPEAN);
    std::cout << "Binomial call: " << bc << "  (BSM: " << res.call << ")\n";
    std::cout << "Binomial put:  " << bp << "  (BSM: " << res.put  << ")\n";

    // American put (early exercise premium)
    double american_put = binomial(S, K, r, sigma, T, 500, OptionType::PUT, ExerciseStyle::AMERICAN);
    std::cout << "\nAmerican put: " << american_put
              << "  Early exercise premium: " << (american_put - bp) << "\n";

    // Implied volatility
    std::cout << "\n--- Implied Volatility ---\n";
    double market_call = res.call;  // use BSM price as market price
    double iv = options::implied_vol(S, K, r, T, market_call, true);
    std::cout << "True sigma: " << sigma << "  Implied: " << iv
              << "  Error: " << std::abs(iv - sigma) << "\n";

    return 0;
}

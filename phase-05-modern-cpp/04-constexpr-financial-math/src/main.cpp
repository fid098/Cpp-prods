#include <iostream>
#include <iomanip>
#include <cmath>
#include "financial_math.hpp"

int main() {
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "=== Constexpr Financial Math ===\n\n";

    // Compile-time computations (checked by static_assert in header)
    constexpr double ANNUAL  = 0.05;
    constexpr double DAILY   = annual_to_daily(ANNUAL);
    constexpr double DF_10YR = discount_factor(ANNUAL, 10.0);
    constexpr double FV_10YR = future_value(1000.0, ANNUAL, 10.0);

    std::cout << "Annual rate:       " << ANNUAL   << "\n";
    std::cout << "Daily rate:        " << DAILY    << "\n";
    std::cout << "Discount factor:   " << DF_10YR  << " (e^(-5%)^10)\n";
    std::cout << "Future value $1000:" << FV_10YR  << "\n";

    std::cout << "\nUnit conversions:\n";
    std::cout << "150 bps = " << bps_to_pct(150) * 100 << "%\n";
    std::cout << "2.5%    = " << pct_to_bps(0.025)     << " bps\n";

    std::cout << "\nconstexpr_exp vs std::exp:\n";
    std::cout << "constexpr_exp(1.0) = " << constexpr_exp(1.0) << "\n";
    std::cout << "std::exp(1.0)      = " << std::exp(1.0)      << "\n";

    return 0;
}

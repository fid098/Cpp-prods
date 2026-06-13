#include <iostream>
#include <iomanip>
#include <vector>
#include "var_calculator.hpp"
#include "portfolio_snapshot.hpp"
#include "position_sizer.hpp"
#include "risk_ratios.hpp"

int main() {
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "=== Sanitizer Risk Lab ===\n\n";

    // Bug 1 — VaR calculation (will corrupt stack / crash under ASan)
    double returns[] = {-0.02, 0.01, -0.05, 0.03, -0.01, 0.02, -0.03, 0.04, -0.04, 0.005};
    std::cout << "VaR 95%: " << compute_var_array(returns, 10, 0.95) << "\n";

    // Bug 2 — Portfolio snapshot (use-after-free under ASan)
    auto* snap = make_snapshot("PORT_A", 1'000'000.0);
    std::cout << "NAV before: " << snap->nav << "\n";
    refresh_snapshot(snap, 1'050'000.0);
    std::cout << "NAV after:  " << snap->nav << "\n";
    delete snap;

    // Bug 3 — Position sizer (overflow)
    int notional   = 2'000'000;
    int multiplier = 2000;
    std::cout << "\nPosition size (should be 4,000,000,000): "
              << size_position(notional, multiplier) << "\n";

    // Bug 4 — Sharpe with zero vol
    std::cout << "Sharpe (zero vol, should not be inf): "
              << sharpe_ratio_buggy(0.1, 0.0) << "\n";

    return 0;
}

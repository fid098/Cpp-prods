#include <iostream>
#include "risk_metrics.hpp"

int main() {
    std::cout << "=== Risk Engine: Reference vs Pointer ===\n\n";

    RiskMetrics m{};
    std::cout << "Initial: " << format_metrics(m) << "\n\n";

    // TODO: demonstrate update_by_value does NOT change m
    std::cout << "[1] update_by_value(m, 0.05, 1.2, 1.8) — m should be UNCHANGED:\n";
    update_by_value(m, 0.05, 1.2, 1.8);
    std::cout << "  " << format_metrics(m) << "\n\n";

    // TODO: demonstrate update_by_pointer DOES change m
    std::cout << "[2] update_by_pointer(&m, 0.05, 1.2, 1.8) — m should CHANGE:\n";
    update_by_pointer(&m, 0.05, 1.2, 1.8);
    std::cout << "  " << format_metrics(m) << "\n\n";

    // TODO: demonstrate update_by_reference DOES change m
    std::cout << "[3] update_by_reference(m, 0.03, 0.95, 2.1) — m should CHANGE:\n";
    update_by_reference(m, 0.03, 0.95, 2.1);
    std::cout << "  " << format_metrics(m) << "\n\n";

    // Null pointer guard test
    std::cout << "[4] Passing nullptr to update_by_pointer — should not crash:\n";
    update_by_pointer(nullptr, 0.0, 0.0, 0.0);
    std::cout << "  OK (null guard worked)\n";

    return 0;
}

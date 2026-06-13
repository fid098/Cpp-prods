#include <iostream>
#include <iomanip>
#include "pnl_tracker.hpp"
#include "order_manager.hpp"

int main() {
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "=== Trading Bug Lab ===\n\n";

    // Bug 1 demo
    double prices[] = {100.0, 102.5, 101.0, 105.0, 103.5};
    std::cout << "Average price (should be ~102.4): "
              << compute_average_price(prices, 5) << "\n";

    // Bug 2 demo
    Trade trades[] = {
        {1000000, 500.0},
        {1000000, 500.0}
    };
    std::cout << "Total PnL (should be 1,000,000,000): "
              << compute_total_pnl(trades, 2) << "\n\n";

    // Bug 3 demo
    OrderManager mgr;
    mgr.add_order(1, 100, 175.0);
    bool filled = mgr.fill_order(1, 100);  // available == requested — should fill
    std::cout << "Order filled (should be true): " << std::boolalpha << filled << "\n";

    // Bug 4 demo — will crash for unknown order
    std::cout << "Order 99 price (should throw): ";
    try {
        double p = mgr.get_order_price(99);
        std::cout << p << " (BUG: should have thrown)\n";
    } catch (const std::exception& e) {
        std::cout << "caught: " << e.what() << "\n";
    }

    return 0;
}

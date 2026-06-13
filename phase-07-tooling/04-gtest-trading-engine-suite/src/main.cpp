#include <iostream>
#include <iomanip>
#include "trading_engine.hpp"

int main() {
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "=== GTest Trading Engine Demo ===\n\n";

    SimpleRiskChecker risk{500'000.0};
    TradingEngine engine{&risk};

    engine.submit_order(100, 175.0);
    engine.submit_order(200, 174.5);
    engine.submit_order(50,  175.5);

    std::cout << "Orders accepted: " << engine.book().order_count() << "\n";
    std::cout << "Best bid: " << engine.book().best_bid() << "\n";

    // This order exceeds notional — rejected
    bool accepted = engine.submit_order(10000, 175.0);
    std::cout << "Large order accepted: " << std::boolalpha << accepted << "\n";

    return 0;
}

#include <iostream>
#include <iomanip>
#include "order_book.hpp"

int main() {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "=== Limit Order Book ===\n\n";

    OrderBook book;
    book.add_order(1, true,  99.50, 200);
    book.add_order(2, true,  99.25, 500);
    book.add_order(3, false, 100.00, 300);
    book.add_order(4, false, 100.25, 150);

    book.print_book();

    std::cout << "Best bid:  " << book.best_bid() << "\n";
    std::cout << "Best ask:  " << book.best_ask() << "\n";
    std::cout << "Spread:    " << book.spread()   << "\n";

    std::cout << "\nCancelling order 1 (best bid)...\n";
    book.cancel_order(1);
    book.print_book();

    return 0;
}

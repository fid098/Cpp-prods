#include <iostream>
#include "portfolio.hpp"

int main() {
    std::cout << "=== Portfolio & Position Model ===\n\n";

    Portfolio p;
    p.add_position({"AAPL", 100, 175.0});
    p.add_position({"MSFT", 200, 290.0});
    p.add_position({"TSLA",  50, 700.0});

    std::cout << "Original portfolio:\n";
    p.print_report();
    std::cout << "Total: " << p.total_value() << "\n\n";

    // Copy and mutate
    Portfolio copy = p;
    // TODO: mutate one position in copy; verify original is unchanged

    return 0;
}

#include <iostream>
#include <iomanip>
#include "equity.hpp"
#include "bond.hpp"
#include "european_option.hpp"

void print_sheet(const Instrument& inst) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  " << inst.name() << "  price=" << inst.price() << "\n";
}

int main() {
    std::cout << "=== Instrument Pricing Domain ===\n\n";

    Equity eq{"AAPL", 100, 175.0};
    Bond   bond{"T10Y", 1000.0, 0.045, 0.04, 10};
    EuropeanOption call{"AAPL", EuropeanOption::Type::CALL,
                        175.0, 180.0, 0.05, 0.20, 0.25};
    EuropeanOption put{"AAPL",  EuropeanOption::Type::PUT,
                        175.0, 180.0, 0.05, 0.20, 0.25};

    std::cout << "Portfolio sheet:\n";
    print_sheet(eq);
    print_sheet(bond);
    print_sheet(call);
    print_sheet(put);

    return 0;
}

#include <iostream>
#include <iomanip>
#include <string>
#include "regime_classifier.hpp"

int main() {
    // One counter per regime (index matches the enum value)
    int counts[4] = {0, 0, 0, 0};
    int day = 0;

    std::cout << "Market Regime Classifier\n";
    std::cout << "Enter daily returns (%), one per line. Type 'done' to finish.\n\n";

    std::string token;
    while (std::cin >> token) {
        if (token == "done") break;

        // TODO: convert token to double with std::stod()
        // TODO: call classify() to get the Regime
        // TODO: increment day, print the result, update counts[]
        (void)day;
    }

    // TODO: print the summary block
    // Hint: use static_cast<int>(Regime::BULL) etc. to index into counts[]

    return 0;
}

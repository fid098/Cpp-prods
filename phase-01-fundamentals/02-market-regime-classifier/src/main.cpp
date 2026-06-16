#include <iostream>
#include <iomanip>
#include <string>
#include "regime_classifier.hpp"

int main()
{
    // One counter per regime (index matches the enum value)
    int counts[4] = {0, 0, 0, 0};
    int day = 0;

    std::cout << "Market Regime Classifier\n";
    std::cout << "Enter daily returns (%), one per line. Type 'done' to finish.\n\n";

    std::string token;
    while (std::cin >> token)
    {
        if (token == "done")
            break;

        // Done: convert token to double with std::stod()
        double token_r = std::stod(token);
        // Done: call classify() to get the Regime
        Regime r = classify(token_r);
        // Done: increment day, print the result, update counts[]
        day++;
        std::cout << "Day " << day << ": " << std::fixed << std::setprecision(2) << (token_r >= 0 ? "+" : "") << token_r << "%"
                  << "  →  " << regime_to_string(r) << "\n";

        counts[static_cast<int>(r)]++;
    }

    // TODO: print the summary block
    // Hint: use static_cast<int>(Regime::BULL) etc. to index into counts[]
    std::cout << "SUMMARY" << "\n";
    std::cout << "BULL:     " << counts[static_cast<int>(Regime::BULL)] << "\n";
    std::cout << "BEAR:     " << counts[static_cast<int>(Regime::BEAR)] << "\n";
    std::cout << "VOLATILE: " << counts[static_cast<int>(Regime::VOLATILE)] << "\n";
    std::cout << "FLAT:     " << counts[static_cast<int>(Regime::FLAT)] << "\n";
}

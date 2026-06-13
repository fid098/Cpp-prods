#include <iostream>
#include <iomanip>
#include "csv_loader.hpp"

int main() {
    std::cout << "=== Exception-Safe CSV Loader ===\n\n";

    // Test 1: file not found
    try {
        auto bars = load_ohlcv("nonexistent.csv");
    } catch (const FileError& e) {
        std::cout << "[Expected] FileError: " << e.what() << "\n";
    }

    // Test 2: create a temp CSV and load it
    {
        std::ofstream f{"test_prices.csv"};
        f << "date,open,high,low,close,volume\n";
        f << "2024-01-01,150.0,155.0,148.0,153.5,1000000\n";
        f << "2024-01-02,153.5,157.0,152.0,156.0,1200000\n";
    }

    try {
        auto bars = load_ohlcv("test_prices.csv");
        std::cout << "Loaded " << bars.size() << " bars.\n";
        std::cout << std::fixed << std::setprecision(2);
        for (const auto& b : bars)
            std::cout << "  " << b.date << "  close=" << b.close << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    // Clean up
    std::remove("test_prices.csv");
    return 0;
}

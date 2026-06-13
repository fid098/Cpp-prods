#include <iostream>
#include <iomanip>
#include <chrono>
#include "monte_carlo.hpp"

int main() {
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "=== Multithreaded Monte Carlo ===\n\n";

    MCParams params{175.0, 180.0, 0.05, 0.20, 0.25};

    auto benchmark = [&](int paths, int threads) {
        auto t0 = std::chrono::high_resolution_clock::now();
        auto r  = mc_call_parallel(params, paths, threads);
        auto t1 = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count();
        std::cout << "  paths=" << std::setw(8) << paths
                  << "  threads=" << threads
                  << "  price=" << r.price
                  << "  time=" << ms << "ms\n";
    };

    benchmark(100000, 1);
    benchmark(100000, 4);
    benchmark(1000000, 8);

    return 0;
}

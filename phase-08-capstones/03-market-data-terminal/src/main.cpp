#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <random>
#include <cmath>
#include <cstdlib>
#include <string>
#include "price_store.hpp"

static constexpr const char* RESET  = "\033[0m";
static constexpr const char* GREEN  = "\033[32m";
static constexpr const char* RED    = "\033[31m";
static constexpr const char* YELLOW = "\033[33m";
static constexpr const char* BOLD   = "\033[1m";
static constexpr const char* CYAN   = "\033[36m";

void render(const PriceStore& store) {
    // Move cursor to top-left without clearing (avoids flicker)
    std::cout << "\033[H";
    std::cout << BOLD << CYAN
              << "  MARKET DATA TERMINAL\n"
              << RESET
              << "  ─────────────────────────────────────────────────────\n"
              << "  " << std::left << std::setw(8) << "SYMBOL"
              << std::right << std::setw(12) << "BID"
              << std::setw(12) << "ASK"
              << std::setw(12) << "LAST"
              << std::setw(10) << "CHG"
              << std::setw(10) << "SPR(bps)"
              << "\n  ─────────────────────────────────────────────────────\n";

    for (int i = 0; i < store.count(); ++i) {
        auto s = store.snapshot(i);
        if (!s.valid) continue;

        double chg = s.last - s.prev_last;
        const char* color = (chg > 0) ? GREEN : (chg < 0) ? RED : YELLOW;
        const char* arrow = (chg > 0) ? " ↑" : (chg < 0) ? " ↓" : " =";

        std::cout << "  " << std::left  << std::setw(8) << s.symbol
                  << std::right << std::fixed << std::setprecision(2)
                  << std::setw(12) << s.bid
                  << std::setw(12) << s.ask
                  << color << std::setw(12) << s.last << RESET
                  << color << std::setw(6) << (chg >= 0 ? "+" : "") << std::setprecision(2) << chg
                  << arrow << RESET
                  << std::setw(8) << std::setprecision(1) << s.spread_bps()
                  << "\n";
    }
    std::cout << "  ─────────────────────────────────────────────────────\n";
    std::cout.flush();
}

int main(int argc, char* argv[]) {
    int run_seconds = (argc >= 2) ? std::atoi(argv[1]) : 30;

    // Register instruments
    PriceStore store;
    store.register_instrument("AAPL",  175.0);
    store.register_instrument("MSFT",  420.0);
    store.register_instrument("TSLA",  250.0);
    store.register_instrument("AMZN",  190.0);
    store.register_instrument("NVDA",  875.0);
    store.register_instrument("GOOGL", 175.0);

    // Clear screen once
    std::cout << "\033[2J";

    // Publisher thread — simulates GBM price ticks
    std::jthread publisher([&](std::stop_token st) {
        thread_local std::mt19937 rng{std::random_device{}()};
        std::normal_distribution<double> norm{0.0, 1.0};

        double prices[6] = {175.0, 420.0, 250.0, 190.0, 875.0, 175.0};
        double vols[6]   = {0.25, 0.22, 0.50, 0.28, 0.45, 0.23};
        double dt = 1.0 / (252.0 * 6.5 * 3600.0);

        while (!st.stop_requested()) {
            for (int i = 0; i < 6; ++i) {
                double Z = norm(rng);
                prices[i] *= std::exp(-0.5 * vols[i] * vols[i] * dt
                                       + vols[i] * std::sqrt(dt) * Z);
                double spread = prices[i] * 0.0002;
                store.update(i, prices[i] - spread, prices[i] + spread, prices[i]);
            }
            std::this_thread::sleep_for(std::chrono::microseconds(500));
        }
    });

    // Renderer — refreshes at 10 Hz
    std::jthread renderer([&](std::stop_token st) {
        while (!st.stop_requested()) {
            render(store);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    std::this_thread::sleep_for(std::chrono::seconds(run_seconds));
    // jthreads stop and join automatically

    std::cout << "\n\nTerminal closed.\n";
    return 0;
}

#include <iostream>
#include <iomanip>
#include "tick_pipeline.hpp"

int main() {
    std::cout << "=== Market Data Pipeline (Move-Aware) ===\n\n";

    // Synthetic batch of 10 ticks (some invalid)
    std::vector<Tick> raw_ticks;
    for (int i = 0; i < 8; ++i)
        raw_ticks.push_back({100.0 + i, 100 + i * 10, 1000000LL + i});
    raw_ticks.push_back({-1.0, 50, 1000008LL});  // invalid price
    raw_ticks.push_back({0.0, 50, 1000009LL});   // invalid price

    TickBatch batch{std::move(raw_ticks), "AAPL_FEED"};
    std::cout << "After ingest:    " << batch.size() << " ticks\n";

    batch = validate_stage(std::move(batch));
    std::cout << "After validate:  " << batch.size() << " ticks (2 filtered)\n";

    double vwap = compute_vwap(batch);
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "VWAP:            " << vwap << "\n";

    batch = enrich_stage(std::move(batch));
    std::cout << "After enrich:    " << batch.size() << " ticks\n";

    return 0;
}

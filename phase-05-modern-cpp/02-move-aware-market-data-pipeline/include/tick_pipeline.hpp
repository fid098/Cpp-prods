#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <utility>

struct Tick {
    double    price{0.0};
    int       volume{0};
    long long timestamp{0};
};

struct TickBatch {
    std::vector<Tick> ticks;
    std::string       source;
    long long         received_at{0};

    TickBatch() = default;
    TickBatch(std::vector<Tick> t, std::string src)
        : ticks{std::move(t)}, source{std::move(src)} {}

    // Move-only: copying is forbidden to avoid accidental expensive copies
    TickBatch(TickBatch&&) = default;
    TickBatch& operator=(TickBatch&&) = default;
    TickBatch(const TickBatch&) = delete;
    TickBatch& operator=(const TickBatch&) = delete;

    std::size_t size() const { return ticks.size(); }
    bool        empty() const { return ticks.empty(); }
};

// Stage 1: filter out ticks with price <= 0
inline TickBatch validate_stage(TickBatch batch) {
    auto& t = batch.ticks;
    t.erase(std::remove_if(t.begin(), t.end(),
        [](const Tick& tick) { return tick.price <= 0.0; }),
        t.end());
    return batch;
}

// Stage 2: compute VWAP and attach to the batch metadata (simplified: return VWAP)
inline double compute_vwap(const TickBatch& batch) {
    if (batch.empty()) return 0.0;
    double notional = 0.0;
    long long total_vol = 0;
    for (const auto& t : batch.ticks) {
        notional  += t.price * t.volume;
        total_vol += t.volume;
    }
    return total_vol > 0 ? notional / total_vol : 0.0;
}

// Stage 3: pass-through with VWAP enrichment (no-op here — just moves)
inline TickBatch enrich_stage(TickBatch batch) {
    // In a real system: annotate each tick with mid-price, bid-ask, etc.
    return batch;
}

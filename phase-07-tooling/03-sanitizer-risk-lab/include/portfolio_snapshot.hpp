#pragma once
#include <ctime>
#include <string>

// ---------------------------------------------------------------------------
// BUG 2: Use-after-free in refresh_snapshot()
//   The old snapshot is deleted, then its fields are written via the pointer.
//   Detected by: -fsanitize=address
//   Fix: update the snapshot BEFORE deleting it, or allocate a new one first.
// ---------------------------------------------------------------------------

struct PortfolioSnapshot {
    std::string portfolio_id;
    double      nav{0.0};
    std::time_t timestamp{0};
};

inline PortfolioSnapshot* make_snapshot(const std::string& id, double nav) {
    auto* s = new PortfolioSnapshot;
    s->portfolio_id = id;
    s->nav          = nav;
    s->timestamp    = std::time(nullptr);
    return s;
}

// BUG 2: deletes snapshot then writes to the freed memory.
inline void refresh_snapshot(PortfolioSnapshot*& snapshot, double new_nav) {
    delete snapshot;
    // BUG 2: snapshot points to freed memory — should allocate a new one first
    snapshot->nav       = new_nav;
    snapshot->timestamp = std::time(nullptr);
}

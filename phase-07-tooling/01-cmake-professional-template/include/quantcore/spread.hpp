#pragma once
#include <stdexcept>

namespace quantcore {

// Bid-ask spread calculator.
struct SpreadMetrics {
    double absolute{0.0};   // ask - bid
    double relative{0.0};   // (ask - bid) / mid
    double mid{0.0};
};

inline SpreadMetrics compute_spread(double bid, double ask) {
    if (bid <= 0.0 || ask <= 0.0)
        throw std::invalid_argument("bid and ask must be positive");
    if (ask < bid)
        throw std::invalid_argument("ask must be >= bid");

    SpreadMetrics m;
    m.absolute = ask - bid;
    m.mid      = (bid + ask) / 2.0;
    m.relative = (m.mid > 0.0) ? m.absolute / m.mid : 0.0;
    return m;
}

} // namespace quantcore

#pragma once
#include <stdexcept>
#include <string>

namespace quantcore {

// Exponential Moving Average price filter.
// alpha in (0, 1]: weight applied to each new price.
// alpha = 1.0 means no smoothing (EMA = raw price).
class PriceFilter {
public:
    explicit PriceFilter(double alpha);

    // Feed a new price observation; returns the updated EMA.
    double update(double price);

    double value()    const { return ema_; }
    bool   is_ready() const { return ready_; }
    double alpha()    const { return alpha_; }

private:
    double alpha_;
    double ema_{0.0};
    bool   ready_{false};
};

} // namespace quantcore

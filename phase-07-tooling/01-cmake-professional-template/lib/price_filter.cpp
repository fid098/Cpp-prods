#include "quantcore/price_filter.hpp"

namespace quantcore {

PriceFilter::PriceFilter(double alpha) : alpha_{alpha} {
    if (alpha <= 0.0 || alpha > 1.0)
        throw std::invalid_argument("alpha must be in (0, 1]");
}

double PriceFilter::update(double price) {
    if (!ready_) {
        ema_   = price;
        ready_ = true;
    } else {
        ema_ = alpha_ * price + (1.0 - alpha_) * ema_;
    }
    return ema_;
}

} // namespace quantcore

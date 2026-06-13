#pragma once
#include <limits>
#include <cstddef>

// Returns (curr - prev) / prev * 100.0
// Precondition: prev != 0
inline double daily_return(double prev, double curr) {
    // TODO
    (void)prev; (void)curr;
    return 0.0;
}

// Returns the highest price in prices[0..n-1]
// Precondition: n >= 1
inline double series_high(const double* prices, std::size_t n) {
    // TODO: start at std::numeric_limits<double>::lowest()
    (void)prices; (void)n;
    return 0.0;
}

// Returns the lowest price in prices[0..n-1]
// Precondition: n >= 1
inline double series_low(const double* prices, std::size_t n) {
    // TODO: start at std::numeric_limits<double>::max()
    (void)prices; (void)n;
    return 0.0;
}

// Returns the maximum drawdown (most negative peak-to-trough %) in the series.
// A flat or rising series returns 0.0.
// Precondition: n >= 2
inline double max_drawdown(const double* prices, std::size_t n) {
    // TODO: track running peak; update max_dd whenever (price - peak)/peak < max_dd
    (void)prices; (void)n;
    return 0.0;
}

// Returns the arithmetic mean of all daily returns in prices[0..n-1].
// Returns 0.0 if n < 2.
inline double average_return(const double* prices, std::size_t n) {
    // TODO: sum daily_return(prices[i-1], prices[i]) for i in 1..n-1, divide by (n-1)
    (void)prices; (void)n;
    return 0.0;
}

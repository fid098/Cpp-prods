#pragma once
#include <limits>
#include <cstddef>

// Returns (curr - prev) / prev * 100.0
// Precondition: prev != 0
inline double daily_return(double prev, double curr)
{
    // Done
    return ((curr - prev) / prev) * 100.0;
}

// Returns the highest price in prices[0..n-1]
// Precondition: n >= 1
inline double series_high(const double *prices, std::size_t n)
{
    // Done: start at std::numeric_limits<double>::lowest()
    double high = prices[0];
    for (std::size_t i = 0; i < n; i++)
    {
        high = std::max(high, prices[i]);
    }
    return high;
}

// Returns the lowest price in prices[0..n-1]
// Precondition: n >= 1
inline double series_low(const double *prices, std::size_t n)
{
    // Done: start at std::numeric_limits<double>::max()
    double low = prices[0];
    for (std::size_t i = 0; i < n; i++)
    {
        low = std::min(low, prices[i]);
    }
    return low;
}

// Returns the maximum drawdown (most negative peak-to-trough %) in the series.
// A flat or rising series returns 0.0.
// Precondition: n >= 2
inline double max_drawdown(const double *prices, std::size_t n)
{
    // Done: track running peak; update max_dd whenever (price - peak)/peak < max_dd
    double max_drawdown{};
    double peak = prices[0];
    double curr{};
    for (std::size_t i = 0; i < n; i++)
    {
        if (prices[i] >= peak)
        {
            peak = prices[i];
        }
        else if (peak > prices[i])
        {
            curr = -((peak - prices[i]) / peak * 100.0);
            max_drawdown = std::min(max_drawdown, curr);
        }
    }
    return max_drawdown;
}

// Returns the arithmetic mean of all daily returns in prices[0..n-1].
// Returns 0.0 if n < 2.
inline double average_return(const double *prices, std::size_t n)
{
    // Done: sum daily_return(prices[i-1], prices[i]) for i in 1..n-1, divide by (n-1)
    if (n < 2)
    {
        return 0.0;
    }
    double sum{};
    double average{};
    for (std::size_t i = 1; i < n; i++)
    {
        sum += daily_return(prices[i - 1], prices[i]);
    }
    average = (sum / (n - 1));
    return average;
}

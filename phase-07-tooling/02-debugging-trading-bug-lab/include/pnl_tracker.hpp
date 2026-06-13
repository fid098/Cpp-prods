#pragma once
#include <cstddef>

// ---------------------------------------------------------------------------
// BUG 1: Off-by-one in compute_average_price()
//   The loop condition uses <= instead of <, reading one element past the end.
//   Compile with -fsanitize=address to catch it.
//
// BUG 2: Integer overflow in compute_total_pnl()
//   The accumulator is `int`, which overflows for large quantity * price.
//   Compile with -fsanitize=undefined to catch it.
//   Fix: change accumulator to long long.
// ---------------------------------------------------------------------------

// Compute the average price over an array of n prices.
inline double compute_average_price(const double* prices, int n) {
    double sum = 0.0;
    // BUG 1: should be i < n, not i <= n
    for (int i = 0; i <= n; ++i) {
        sum += prices[i];
    }
    return sum / n;
}

struct Trade {
    int    quantity;
    double price;
};

// Compute total PnL: sum of quantity * price for each trade.
// BUG 2: int overflows for large quantity * price values.
inline long long compute_total_pnl(const Trade* trades, int n) {
    int total = 0;   // BUG 2: should be long long
    for (int i = 0; i < n; ++i) {
        total += trades[i].quantity * static_cast<int>(trades[i].price);
    }
    return total;
}

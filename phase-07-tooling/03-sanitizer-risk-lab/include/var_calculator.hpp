#pragma once
#include <algorithm>
#include <vector>
#include <stdexcept>

// ---------------------------------------------------------------------------
// BUG 1: Stack buffer overflow in compute_var_array()
//   The copy loop uses i <= n, writing returns[n] into sorted[n] — one past end.
//   Detected by: -fsanitize=address
//   Fix: change i <= n to i < n in both loops.
// ---------------------------------------------------------------------------

// Historical simulation VaR at given confidence level (e.g. 0.95).
// Returns the loss at the confidence percentile (negative return).
inline double compute_var(const std::vector<double>& returns, double confidence) {
    if (returns.empty()) throw std::invalid_argument("empty returns");
    if (confidence <= 0.0 || confidence >= 1.0)
        throw std::invalid_argument("confidence must be in (0, 1)");

    std::vector<double> sorted = returns;
    std::sort(sorted.begin(), sorted.end());
    std::size_t idx = static_cast<std::size_t>((1.0 - confidence) * sorted.size());
    return -sorted[idx];
}

// Raw-array variant — intentionally buggy for the exercise.
inline double compute_var_array(const double* returns, int n, double confidence) {
    if (n <= 0) throw std::invalid_argument("n must be positive");

    // VLA for sorting — intentionally not using std::vector so the overflow is clear
    std::vector<double> sorted(n);

    // BUG 1: should be i < n
    for (int i = 0; i <= n; ++i) {
        sorted[i] = returns[i];   // writes sorted[n] out of bounds!
    }

    std::sort(sorted.begin(), sorted.end());
    int idx = static_cast<int>((1.0 - confidence) * n);
    return -sorted[idx];
}

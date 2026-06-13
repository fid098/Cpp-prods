#pragma once
#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <algorithm>

struct Holding {
    std::string symbol;
    std::string sector;
    int         qty{0};
    double      price{0.0};
    double value() const { return static_cast<double>(qty) * price; }
};

// Returns the total portfolio value.
inline double total_value(const std::vector<Holding>& h) {
    // TODO: std::accumulate with lambda summing h.value()
    (void)h;
    return 0.0;
}

// Returns a vector of weights (fraction of total value) in the same order as h.
inline std::vector<double> weights(const std::vector<Holding>& h) {
    // TODO: std::transform
    (void)h;
    return {};
}

// Returns the Herfindahl-Hirschman Index: Σ weight_i²
inline double herfindahl(const std::vector<Holding>& h) {
    // TODO: accumulate weight^2
    (void)h;
    return 0.0;
}

// Sorts holdings in place by value descending.
inline void sort_by_value(std::vector<Holding>& h) {
    // TODO: std::sort with lambda
    (void)h;
}

// Returns sector → total weight map.
inline std::map<std::string, double> sector_exposure(const std::vector<Holding>& h) {
    // TODO: iterate, accumulate by sector
    (void)h;
    return {};
}

// Returns a pointer to the largest holding, or nullptr if empty.
inline const Holding* largest_holding(const std::vector<Holding>& h) {
    if (h.empty()) return nullptr;
    // TODO: std::max_element with lambda on value()
    return nullptr;
}

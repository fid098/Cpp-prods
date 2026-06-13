#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

struct Stock {
    std::string symbol;
    double momentum{0.0};
    double value{0.0};
    double quality{0.0};
};

struct FactorWeights {
    double momentum{1.0/3};
    double value{1.0/3};
    double quality{1.0/3};
};

inline double composite_score(const Stock& s, const FactorWeights& w) {
    return s.momentum * w.momentum + s.value * w.value + s.quality * w.quality;
}

// Returns stocks sorted by composite score descending.
inline std::vector<Stock> rank_universe(std::vector<Stock> stocks,
                                         const FactorWeights& w) {
    std::sort(stocks.begin(), stocks.end(),
        [&w](const Stock& a, const Stock& b) {
            return composite_score(a, w) > composite_score(b, w);
        });
    return stocks;
}

// Returns the top N stocks by composite score.
inline std::vector<Stock> top_n(std::vector<Stock> stocks, int n,
                                  const FactorWeights& w) {
    if (n >= static_cast<int>(stocks.size()))
        return rank_universe(std::move(stocks), w);
    std::partial_sort(stocks.begin(), stocks.begin() + n, stocks.end(),
        [&w](const Stock& a, const Stock& b) {
            return composite_score(a, w) > composite_score(b, w);
        });
    stocks.resize(n);
    return stocks;
}

// Returns equal-weight allocation for n holdings (1.0/n each).
inline double equal_weight(int n) {
    return n > 0 ? 1.0 / n : 0.0;
}

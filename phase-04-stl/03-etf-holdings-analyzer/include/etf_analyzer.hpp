#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

struct ETFHolding {
    std::string symbol;
    std::string issuer;
    std::string sector;
    double      weight{0.0};  // percentage 0-100
};

// Returns top N holdings by weight (descending).
inline std::vector<ETFHolding> top_n_holdings(std::vector<ETFHolding> h, int n) {
    std::sort(h.begin(), h.end(),
        [](const ETFHolding& a, const ETFHolding& b){ return a.weight > b.weight; });
    if (n < static_cast<int>(h.size())) h.resize(n);
    return h;
}

// Returns sector → total weight.
inline std::map<std::string, double> sector_weights(const std::vector<ETFHolding>& h) {
    std::map<std::string, double> result;
    for (const auto& x : h) result[x.sector] += x.weight;
    return result;
}

// Returns the set of unique issuer names.
inline std::set<std::string> unique_issuers(const std::vector<ETFHolding>& h) {
    std::set<std::string> s;
    for (const auto& x : h) s.insert(x.issuer);
    return s;
}

// Returns a multimap from sector name → ETFHolding.
inline std::multimap<std::string, ETFHolding> holdings_by_sector(
    const std::vector<ETFHolding>& h)
{
    std::multimap<std::string, ETFHolding> result;
    for (const auto& x : h) result.emplace(x.sector, x);
    return result;
}

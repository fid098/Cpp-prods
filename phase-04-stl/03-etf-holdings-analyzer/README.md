# 03 — ETF Holdings Analyzer

> Phase 4 · STL | C++23 | Parse and rank ETF holdings by sector and issuer

## What You'll Build

A tool that reads ETF holdings (symbol, issuer, sector, weight) and ranks them by sector concentration, issuer diversification, and top holdings — using `std::map`, `std::set`, and sorting.

## Learning Objectives

- Use `std::multimap` to group holdings by sector
- Use `std::set` to find unique issuers
- Use `std::map` to aggregate weights by sector
- Sort a `std::vector` with a custom comparator
- Use range-based for loops with structured bindings (`auto& [key, val]`)

## Key Resources

| Concept | Link |
|---------|------|
| std::map | https://en.cppreference.com/w/cpp/container/map |
| std::multimap | https://en.cppreference.com/w/cpp/container/multimap |
| std::set | https://en.cppreference.com/w/cpp/container/set |
| Structured bindings | https://en.cppreference.com/w/cpp/language/structured_bindings |

## Requirements

1. `ETFHolding` struct: symbol, issuer, sector, weight (0–100%)
2. `top_n_holdings(holdings, n)` — return top N by weight
3. `sector_weights(holdings)` — `map<string, double>` of aggregated sector weights
4. `unique_issuers(holdings)` — `set<string>` of all issuers
5. `holdings_by_sector(holdings)` — `multimap<string, ETFHolding>` grouped by sector

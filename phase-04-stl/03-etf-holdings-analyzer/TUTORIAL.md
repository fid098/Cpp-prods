# Tutorial — ETF Holdings Analyzer

## std::set — Unique Issuers

```cpp
#include <set>

std::set<std::string> unique_issuers(const std::vector<ETFHolding>& h) {
    std::set<std::string> issuers;
    for (const auto& holding : h)
        issuers.insert(holding.issuer);  // duplicates are silently ignored
    return issuers;
}
```

`std::set` stores only unique elements in sorted order. `insert` on an existing element is a no-op.

> **Docs:** https://en.cppreference.com/w/cpp/container/set

---

## std::multimap — Group by Sector

```cpp
#include <map>

std::multimap<std::string, ETFHolding> holdings_by_sector(
    const std::vector<ETFHolding>& h)
{
    std::multimap<std::string, ETFHolding> result;
    for (const auto& holding : h)
        result.emplace(holding.sector, holding);
    return result;
}
```

`std::multimap` allows duplicate keys — multiple holdings can share the same sector.

To iterate all holdings in one sector:
```cpp
auto [begin, end] = multimap.equal_range("Tech");
for (auto it = begin; it != end; ++it)
    std::cout << it->second.symbol << "\n";
```

> **Docs:** https://en.cppreference.com/w/cpp/container/multimap

---

## Top N Holdings

```cpp
std::vector<ETFHolding> top_n_holdings(std::vector<ETFHolding> h, int n) {
    std::sort(h.begin(), h.end(),
        [](const ETFHolding& a, const ETFHolding& b) {
            return a.weight > b.weight;  // descending
        });
    if (n < static_cast<int>(h.size())) h.resize(n);
    return h;
}
```

---

## Structured Bindings (C++17+)

```cpp
for (const auto& [sector, weight] : sector_weights(holdings))
    std::cout << sector << ": " << weight << "%\n";
```

`auto& [key, val]` unpacks a `std::pair` from a map range.

> **Docs:** https://en.cppreference.com/w/cpp/language/structured_bindings

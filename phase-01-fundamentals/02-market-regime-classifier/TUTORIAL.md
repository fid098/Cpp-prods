# Tutorial — Market Regime Classifier

## Step 1 — Define the Regime Enum

Open `include/regime_classifier.hpp`. An **enum class** gives named integer constants that the compiler treats as a distinct type — you can't accidentally mix them up with plain `int` values.

```cpp
#pragma once
#include <cmath>    // std::abs
#include <string>

enum class Regime {
    BULL,
    BEAR,
    VOLATILE,
    FLAT
};
```

> **Docs:** https://en.cppreference.com/w/cpp/language/enum
>
> Use `enum class` (scoped enum) rather than plain `enum`. It avoids name collisions and forces you to write `Regime::BULL` instead of just `BULL`.

---

## Step 2 — The classify() Function

```cpp
// Classifies a daily return (in percent) into a market regime.
inline Regime classify(double return_pct) {
    // TODO: check |return_pct| > 2.0 → VOLATILE
    //       check return_pct >= 1.0  → BULL
    //       check return_pct <= -1.0 → BEAR
    //       otherwise                 → FLAT
    return Regime::FLAT; // placeholder
}
```

The key tool here is `std::abs`:

```cpp
if (std::abs(return_pct) > 2.0) {
    return Regime::VOLATILE;
}
```

Because `std::abs` for floating-point lives in `<cmath>`, make sure you `#include <cmath>` at the top.

> **Docs:** https://en.cppreference.com/w/cpp/numeric/math/abs

---

## Step 3 — Convert Regime to String

Add a helper so you can print the regime label:

```cpp
inline std::string regime_to_string(Regime r) {
    switch (r) {
        case Regime::BULL:     return "BULL";
        case Regime::BEAR:     return "BEAR";
        case Regime::VOLATILE: return "VOLATILE";
        case Regime::FLAT:     return "FLAT";
    }
    return "UNKNOWN"; // unreachable, but the compiler may warn without it
}
```

`switch` dispatches on an integer-like value. With `enum class` you must write the full name (`Regime::BULL`). If you leave out a case, the compiler can warn you with `-Wswitch` — a useful safety net.

> **Docs:** https://en.cppreference.com/w/cpp/language/switch

---

## Step 4 — Input Loop in main.cpp

```cpp
#include <iostream>
#include <iomanip>
#include <string>
#include "regime_classifier.hpp"

int main() {
    int counts[4] = {0, 0, 0, 0}; // BULL, BEAR, VOLATILE, FLAT
    int day = 0;

    std::cout << "Market Regime Classifier\n";
    std::cout << "Enter daily returns (%), one per line. Type 'done' to finish.\n\n";

    std::string token;
    while (std::cin >> token) {
        if (token == "done") break;

        double ret = std::stod(token); // convert string → double
        Regime r   = classify(ret);
        ++day;

        std::cout << "  Day " << day << ":  "
                  << std::fixed << std::setprecision(2)
                  << (ret >= 0 ? "+" : "") << ret << "%"
                  << "  →  " << regime_to_string(r) << "\n";

        // TODO: increment the right counter in counts[]
    }

    // TODO: print summary
    return 0;
}
```

`std::stod` converts a `std::string` to a `double`.
> **Docs:** https://en.cppreference.com/w/cpp/string/basic_string/stod

---

## Step 5 — Counting Regimes

The simplest approach is a fixed-size array of counters (one per regime):

```cpp
// Increment counter using the enum value as an index:
counts[static_cast<int>(r)]++;
```

Then print them in the summary:

```cpp
std::cout << "\n=== Summary ===\n";
std::cout << "  Bull:     " << counts[static_cast<int>(Regime::BULL)]     << " day(s)\n";
std::cout << "  Bear:     " << counts[static_cast<int>(Regime::BEAR)]     << " day(s)\n";
std::cout << "  Volatile: " << counts[static_cast<int>(Regime::VOLATILE)] << " day(s)\n";
std::cout << "  Flat:     " << counts[static_cast<int>(Regime::FLAT)]     << " day(s)\n";
std::cout << "  Total:    " << day                                         << " day(s)\n";
```

---

## Step 6 — Verify with Tests

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
cd build && ctest --output-on-failure
```

The tests check every boundary: exactly at 1.0%, just below 1.0%, exactly at -2.0%, etc. Boundary conditions are where classification bugs hide.

---

## Common Pitfalls

| Mistake | Fix |
|---------|-----|
| Using `abs()` instead of `std::abs()` | `abs()` from `<cstdlib>` works on integers; use `std::abs()` from `<cmath>` for `double` |
| Wrong order of conditions | Always check VOLATILE first; a 3% move is both VOLATILE and could look like BULL |
| Falling through `switch` without `break` | Each `case` needs either `return` or `break` |

---

## What's Next

Move on to **Project 03 — Trade Ticket Validator** where you'll meet `std::string` operations, function overloads, and boolean logic.

#pragma once
#include <cmath>
#include <string>

enum class Regime {
    BULL,
    BEAR,
    VOLATILE,
    FLAT
};

// Classifies a daily percentage return into a market regime.
// Rules (apply in this order):
//   |return_pct| > 2.0  → VOLATILE
//   return_pct  >= 1.0  → BULL
//   return_pct  <= -1.0 → BEAR
//   otherwise           → FLAT
inline Regime classify(double return_pct) {
    // TODO: implement using if/else if/else
    (void)return_pct;
    return Regime::FLAT; // placeholder
}

// Converts a Regime value to its display string.
inline std::string regime_to_string(Regime r) {
    // TODO: implement using switch
    (void)r;
    return "UNKNOWN"; // placeholder
}

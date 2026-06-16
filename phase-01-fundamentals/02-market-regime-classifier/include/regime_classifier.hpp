#pragma once
#include <cmath>
#include <string>

enum class Regime
{
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

inline Regime classify(double return_pct)
{
    // Done: implement using if/else if/else
    if (std::abs(return_pct) > 2.0)
    {
        return Regime::VOLATILE;
    }
    else
    {
        if (return_pct >= 1.0)
        {
            return Regime::BULL;
        }
        else if (return_pct <= -1.0)
        {
            return Regime::BEAR;
        }
        else
        {
            return Regime::FLAT;
        }
    }
}

// Converts a Regime value to its display string.
inline std::string regime_to_string(Regime r)
{
    // Done: implement using switch
    switch (r)
    {
    case Regime::BULL:
        return "BULL";
    case Regime::BEAR:
        return "BEAR";
    case Regime::VOLATILE:
        return "VOLATILE";
    case Regime::FLAT:
        return "FLAT";
    }
}

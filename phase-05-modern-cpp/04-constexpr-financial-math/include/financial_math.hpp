#pragma once
#include <cmath>

// ── Unit conversions (all constexpr) ─────────────────────────────────────────

// Basis points to percentage (e.g. 100bps = 1%)
constexpr double bps_to_pct(double bps) { return bps / 10000.0; }

// Percentage to basis points
constexpr double pct_to_bps(double pct) { return pct * 10000.0; }

// Annual rate to daily rate (365-day year)
constexpr double annual_to_daily(double annual) { return annual / 365.0; }

// Daily rate to annual rate
constexpr double daily_to_annual(double daily) { return daily * 365.0; }

// ── Compile-time Taylor series exp ───────────────────────────────────────────

constexpr double constexpr_exp(double x, int terms = 20) {
    double result = 1.0, term = 1.0;
    for (int n = 1; n < terms; ++n) {
        term   *= x / n;
        result += term;
    }
    return result;
}

// ── Financial functions (constexpr) ──────────────────────────────────────────

// Continuous discount factor: e^(-rate * T)
constexpr double discount_factor(double rate, double T) {
    return constexpr_exp(-rate * T);
}

// Future value: PV * e^(rate * T)
constexpr double future_value(double pv, double rate, double T) {
    return pv * constexpr_exp(rate * T);
}

// Annualised vol from daily vol
constexpr double daily_vol_to_annual(double daily_vol) {
    return daily_vol * constexpr_exp(0.5 * std::log(252.0));
}

// ── consteval: must be evaluated at compile time ──────────────────────────────

// Returns the number of calendar days in the given year.
consteval int days_in_year(bool leap_year) {
    return leap_year ? 366 : 365;
}

// ── Compile-time assertions ───────────────────────────────────────────────────

static_assert(bps_to_pct(10000.0) == 1.0,   "10000 bps must be 100%");
static_assert(pct_to_bps(1.0)     == 10000.0);
static_assert(days_in_year(false)  == 365);
static_assert(days_in_year(true)   == 366);

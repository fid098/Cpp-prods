#pragma once
#include <stdexcept>

// ---------------------------------------------------------------------------
// BUG 4: Division by zero in sharpe_ratio_buggy()
//   Dividing by std_dev when it is 0.0 is undefined behaviour for integers
//   and produces ±inf for doubles — UBSan catches the integer version.
//   Fix: guard with if (std_dev == 0.0) return 0.0;
// ---------------------------------------------------------------------------

// BUG 4: no guard against zero std_dev
inline double sharpe_ratio_buggy(double mean_return, double std_dev,
                                  double risk_free_rate = 0.0) {
    return (mean_return - risk_free_rate) / std_dev;  // UB / inf if std_dev==0
}

// Fixed version — TODO: add the guard
inline double sharpe_ratio(double mean_return, double std_dev,
                            double risk_free_rate = 0.0) {
    // TODO: if (std_dev == 0.0) return 0.0;
    return (mean_return - risk_free_rate) / std_dev;
}

inline double sortino_ratio(double mean_return, double downside_std_dev,
                             double risk_free_rate = 0.0) {
    if (downside_std_dev == 0.0) return 0.0;
    return (mean_return - risk_free_rate) / downside_std_dev;
}

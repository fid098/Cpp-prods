#pragma once
#include <string>
#include <sstream>
#include <iomanip>

struct RiskMetrics {
    double var{0.0};
    double beta{0.0};
    double sharpe{0.0};
    double alpha{0.0};
    double max_dd{0.0};
};

// Pass-by-value: modifies a LOCAL copy — caller's m is unchanged.
inline void update_by_value(RiskMetrics m,
                             double var, double beta, double sharpe) {
    m.var = var; m.beta = beta; m.sharpe = sharpe;
}

// Pass-by-pointer: modifies *m in place. Guard against nullptr.
inline void update_by_pointer(RiskMetrics* m,
                               double var, double beta, double sharpe) {
    // Done: if (!m) return; then set m->var, m->beta, m->sharpe
    if (!m){
        return;
    }
    else{
        m->var = var; m->beta = beta; m->sharpe = sharpe;
    }
}

// Pass-by-reference: modifies m directly (no pointer syntax for the caller).
inline void update_by_reference(RiskMetrics& m,
                                 double var, double beta, double sharpe) {
    // Done: set m.var, m.beta, m.sharpe
    m.var = var; m.beta = beta; m.sharpe = sharpe;
}

// Read-only const ref: returns formatted string, no copy of m.
inline std::string format_metrics(const RiskMetrics& m) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "var=" << m.var << "  beta=" << m.beta
       << "  sharpe=" << m.sharpe << "  alpha=" << m.alpha
       << "  max_dd=" << m.max_dd;
    return ss.str();
}

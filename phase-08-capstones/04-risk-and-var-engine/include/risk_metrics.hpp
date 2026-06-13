#pragma once
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdexcept>
#include <future>
#include <random>

namespace risk {

// ---- Descriptive statistics -----------------------------------------------

inline double mean(const std::vector<double>& v) {
    if (v.empty()) throw std::invalid_argument("empty vector");
    return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
}

inline double variance(const std::vector<double>& v, bool sample = true) {
    double m = mean(v);
    double sum = 0.0;
    for (double x : v) sum += (x - m) * (x - m);
    return sum / (v.size() - (sample ? 1 : 0));
}

inline double stddev(const std::vector<double>& v, bool sample = true) {
    return std::sqrt(variance(v, sample));
}

inline double covariance(const std::vector<double>& x,
                          const std::vector<double>& y, bool sample = true) {
    if (x.size() != y.size()) throw std::invalid_argument("size mismatch");
    double mx = mean(x), my = mean(y);
    double sum = 0.0;
    for (std::size_t i = 0; i < x.size(); ++i)
        sum += (x[i] - mx) * (y[i] - my);
    return sum / (x.size() - (sample ? 1 : 0));
}

inline double pearson(const std::vector<double>& x, const std::vector<double>& y) {
    double sx = stddev(x), sy = stddev(y);
    if (sx < 1e-12 || sy < 1e-12) return 0.0;
    return covariance(x, y) / (sx * sy);
}

// ---- VaR methods ----------------------------------------------------------

// Historical simulation VaR. confidence = 0.95 → 95% VaR (loss at 5th percentile).
inline double historical_var(std::vector<double> returns, double confidence) {
    if (returns.empty()) throw std::invalid_argument("empty returns");
    std::sort(returns.begin(), returns.end());
    std::size_t idx = static_cast<std::size_t>((1.0 - confidence) * returns.size());
    idx = std::min(idx, returns.size() - 1);
    return -returns[idx];
}

// Expected Shortfall (CVaR): mean of returns worse than VaR.
inline double expected_shortfall(std::vector<double> returns, double confidence) {
    if (returns.empty()) throw std::invalid_argument("empty returns");
    std::sort(returns.begin(), returns.end());
    int cut = static_cast<int>((1.0 - confidence) * returns.size());
    if (cut <= 0) return -returns.front();
    double sum = 0.0;
    for (int i = 0; i < cut; ++i) sum += returns[i];
    return -sum / cut;
}

// Parametric (normal) VaR. z_alpha for 95% ≈ 1.6449, for 99% ≈ 2.3263.
inline double parametric_var(double mean_ret, double vol, double confidence) {
    // Rational approximation for inverse normal CDF (Abramowitz & Stegun 26.2.17)
    auto inv_norm = [](double p) {
        double t = std::sqrt(-2.0 * std::log(p));
        double c0 = 2.515517, c1 = 0.802853, c2 = 0.010328;
        double d1 = 1.432788, d2 = 0.189269, d3 = 0.001308;
        return t - (c0 + c1*t + c2*t*t) / (1 + d1*t + d2*t*t + d3*t*t*t);
    };
    double z = inv_norm(1.0 - confidence);
    return -(mean_ret - z * vol);
}

// Monte Carlo VaR (parallelised).
inline double mc_var(double mean_ret, double vol, double confidence,
                      int paths = 100000, int threads = 4) {
    int per_thread = paths / threads;
    std::vector<std::future<std::vector<double>>> futures;

    for (int t = 0; t < threads; ++t) {
        futures.push_back(std::async(std::launch::async, [=]() {
            thread_local std::mt19937 rng{std::random_device{}()};
            std::normal_distribution<double> dist{mean_ret, vol};
            std::vector<double> samples(per_thread);
            for (auto& s : samples) s = dist(rng);
            return samples;
        }));
    }

    std::vector<double> all;
    all.reserve(paths);
    for (auto& f : futures) {
        auto v = f.get();
        all.insert(all.end(), v.begin(), v.end());
    }
    return historical_var(all, confidence);
}

// ---- Portfolio metrics -----------------------------------------------------

inline double portfolio_beta(const std::vector<double>& port_returns,
                               const std::vector<double>& market_returns) {
    double var_m = variance(market_returns);
    if (var_m < 1e-12) return 0.0;
    return covariance(port_returns, market_returns) / var_m;
}

inline double max_drawdown(const std::vector<double>& equity_curve) {
    if (equity_curve.empty()) return 0.0;
    double peak = equity_curve[0], max_dd = 0.0;
    for (double eq : equity_curve) {
        if (eq > peak) peak = eq;
        if (peak > 0.0) max_dd = std::max(max_dd, (peak - eq) / peak);
    }
    return max_dd;
}

inline double hhi(const std::vector<double>& weights) {
    return std::inner_product(weights.begin(), weights.end(), weights.begin(), 0.0);
}

inline double annualised_sharpe(const std::vector<double>& daily_returns,
                                  double risk_free = 0.0) {
    double m = mean(daily_returns) - risk_free / 252.0;
    double s = stddev(daily_returns);
    if (s < 1e-12) return 0.0;
    return m / s * std::sqrt(252.0);
}

inline std::vector<std::vector<double>>
correlation_matrix(const std::vector<std::vector<double>>& returns) {
    int n = static_cast<int>(returns.size());
    std::vector<std::vector<double>> corr(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        corr[i][i] = 1.0;
        for (int j = 0; j < i; ++j) {
            corr[i][j] = corr[j][i] = pearson(returns[i], returns[j]);
        }
    }
    return corr;
}

} // namespace risk

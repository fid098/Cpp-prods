#pragma once
#include <cmath>
#include <random>
#include <future>
#include <vector>
#include <algorithm>

struct MCParams {
    double S;     // spot price
    double K;     // strike
    double r;     // risk-free rate
    double sigma; // volatility
    double T;     // time to expiry (years)
};

struct MCResult {
    double price{0.0};
    double std_error{0.0};
    int    paths{0};
};

// Single-threaded Monte Carlo European call pricer.
inline double mc_call_single(const MCParams& p, int paths, std::mt19937& rng) {
    std::normal_distribution<double> norm{0.0, 1.0};
    double sum = 0.0, sum_sq = 0.0;
    for (int i = 0; i < paths; ++i) {
        double Z  = norm(rng);
        double ST = p.S * std::exp((p.r - 0.5*p.sigma*p.sigma)*p.T
                                    + p.sigma*std::sqrt(p.T)*Z);
        double payoff = std::max(ST - p.K, 0.0);
        sum    += payoff;
        sum_sq += payoff * payoff;
    }
    double df    = std::exp(-p.r * p.T);
    double mean  = sum / paths;
    double var   = (sum_sq / paths - mean * mean) / paths;
    return df * mean;
}

// Parallel Monte Carlo using std::async.
inline MCResult mc_call_parallel(const MCParams& p, int total_paths, int num_threads) {
    int per_thread = total_paths / num_threads;
    std::vector<std::future<double>> futures;

    for (int t = 0; t < num_threads; ++t) {
        futures.push_back(std::async(std::launch::async, [p, per_thread]() {
            thread_local std::mt19937 rng{std::random_device{}()};
            return mc_call_single(p, per_thread, rng);
        }));
    }

    double total = 0.0;
    for (auto& f : futures) total += f.get();

    MCResult r;
    r.price = total / num_threads;
    r.paths = total_paths;
    return r;
}

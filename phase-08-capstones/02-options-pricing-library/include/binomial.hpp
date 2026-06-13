#pragma once
#include <cmath>
#include <vector>
#include <algorithm>
#include <stdexcept>

namespace options {

enum class OptionType { CALL, PUT };
enum class ExerciseStyle { EUROPEAN, AMERICAN };

// Cox-Ross-Rubinstein binomial tree pricer.
inline double binomial(double S, double K, double r, double sigma, double T,
                        int steps, OptionType type, ExerciseStyle style) {
    if (steps <= 0) throw std::invalid_argument("steps must be positive");

    double dt = T / steps;
    double u  = std::exp(sigma * std::sqrt(dt));
    double d  = 1.0 / u;
    double p  = (std::exp(r * dt) - d) / (u - d);
    double df = std::exp(-r * dt);

    if (p <= 0.0 || p >= 1.0)
        throw std::runtime_error("risk-neutral probability out of range");

    // Terminal payoffs
    std::vector<double> V(steps + 1);
    for (int i = 0; i <= steps; ++i) {
        double spot = S * std::pow(u, i) * std::pow(d, steps - i);
        V[i] = (type == OptionType::CALL)
               ? std::max(spot - K, 0.0)
               : std::max(K - spot, 0.0);
    }

    // Backward induction
    for (int t = steps - 1; t >= 0; --t) {
        for (int i = 0; i <= t; ++i) {
            double continuation = df * (p * V[i+1] + (1.0 - p) * V[i]);
            if (style == ExerciseStyle::AMERICAN) {
                double spot = S * std::pow(u, i) * std::pow(d, t - i);
                double intrinsic = (type == OptionType::CALL)
                                   ? std::max(spot - K, 0.0)
                                   : std::max(K - spot, 0.0);
                V[i] = std::max(continuation, intrinsic);
            } else {
                V[i] = continuation;
            }
        }
    }
    return V[0];
}

} // namespace options

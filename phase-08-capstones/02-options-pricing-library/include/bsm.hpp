#pragma once
#include <cmath>
#include <stdexcept>
#include <numbers>

namespace options {

inline double norm_cdf(double x) {
    return 0.5 * std::erfc(-x / std::numbers::sqrt2);
}

inline double norm_pdf(double x) {
    return std::exp(-0.5 * x * x) / std::sqrt(2.0 * std::numbers::pi);
}

struct BSMResult {
    double d1, d2;
    double call, put;
    double delta_call, delta_put;
    double gamma, vega, theta_call, theta_put, rho_call, rho_put;
};

// Full Black-Scholes-Merton pricing for European options.
// S=spot, K=strike, r=risk-free rate, sigma=vol, T=time to expiry (years)
inline BSMResult bsm(double S, double K, double r, double sigma, double T) {
    if (S <= 0 || K <= 0 || sigma <= 0 || T <= 0)
        throw std::invalid_argument("S, K, sigma, T must all be positive");

    BSMResult res;
    res.d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    res.d2 = res.d1 - sigma * std::sqrt(T);

    double Nd1  = norm_cdf(res.d1);
    double Nd2  = norm_cdf(res.d2);
    double Nmd1 = norm_cdf(-res.d1);
    double Nmd2 = norm_cdf(-res.d2);
    double df   = std::exp(-r * T);
    double pdf1 = norm_pdf(res.d1);
    double sqT  = std::sqrt(T);

    res.call = S * Nd1 - K * df * Nd2;
    res.put  = K * df * Nmd2 - S * Nmd1;

    res.delta_call = Nd1;
    res.delta_put  = Nd1 - 1.0;
    res.gamma      = pdf1 / (S * sigma * sqT);
    res.vega        = S * pdf1 * sqT;                                       // per unit vol
    res.theta_call  = -(S * pdf1 * sigma / (2.0 * sqT)) - r * K * df * Nd2;
    res.theta_put   = -(S * pdf1 * sigma / (2.0 * sqT)) + r * K * df * Nmd2;
    res.rho_call    =  K * T * df * Nd2;
    res.rho_put     = -K * T * df * Nmd2;

    return res;
}

// Implied volatility via Newton-Raphson.
inline double implied_vol(double S, double K, double r, double T,
                           double market_price, bool is_call = true,
                           double tol = 1e-7, int max_iter = 200) {
    double sigma = 0.20;
    for (int i = 0; i < max_iter; ++i) {
        auto res   = bsm(S, K, r, sigma, T);
        double px  = is_call ? res.call : res.put;
        double veg = res.vega;
        double err = px - market_price;
        if (std::abs(err) < tol) return sigma;
        if (veg < 1e-12) break;
        sigma -= err / veg;
        sigma  = std::max(1e-6, std::min(sigma, 10.0));
    }
    throw std::runtime_error("implied vol did not converge");
}

} // namespace options

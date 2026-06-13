# Tutorial — Options Pricing Library

## Step 1 — Black-Scholes Formula

For a European call on a non-dividend-paying stock:

```
C = S·N(d1) - K·e^(-rT)·N(d2)
d1 = [ln(S/K) + (r + σ²/2)·T] / (σ·√T)
d2 = d1 - σ·√T
```

In C++, `N(x)` = standard normal CDF = `0.5 * erfc(-x / sqrt(2))`:

```cpp
#include <cmath>
auto N = [](double x) { return 0.5 * std::erfc(-x / std::sqrt(2.0)); };
```

Put price via put-call parity: `P = C - S + K·e^(-rT)`

> **Docs:** https://en.cppreference.com/w/cpp/numeric/math/erfc

---

## Step 2 — Greeks (Analytical)

```cpp
// Delta: dC/dS
double delta_call = N(d1);
double delta_put  = N(d1) - 1.0;

// Gamma: d²C/dS² (same for call and put)
double pdf_d1 = std::exp(-0.5*d1*d1) / std::sqrt(2.0*M_PI);
double gamma  = pdf_d1 / (S * sigma * std::sqrt(T));

// Vega: dC/dσ (same for call and put, per 1% vol move divide by 100)
double vega = S * pdf_d1 * std::sqrt(T);

// Theta: dC/dT (per calendar day divide by 365)
double theta_call = -(S * pdf_d1 * sigma / (2*std::sqrt(T)))
                    - r * K * std::exp(-r*T) * N(d2);
```

---

## Step 3 — Implied Volatility (Newton-Raphson)

Given a market price `market_price`, find `sigma` such that `BSM(sigma) == market_price`:

```cpp
double implied_vol(double S, double K, double r, double T,
                   double market_price, double tol = 1e-6) {
    double sigma = 0.2;  // initial guess
    for (int i = 0; i < 100; ++i) {
        double price = bsm_call(S, K, r, sigma, T);
        double vega  = bsm_vega(S, K, r, sigma, T);
        double diff  = price - market_price;
        if (std::abs(diff) < tol) return sigma;
        if (vega < 1e-12) break;  // prevent division by zero
        sigma -= diff / vega;
        sigma = std::max(1e-6, sigma);  // keep vol positive
    }
    throw std::runtime_error("implied vol did not converge");
}
```

---

## Step 4 — Binomial Tree (CRR)

Cox-Ross-Rubinstein parameters:
```
u = e^(σ√Δt)
d = 1/u
p = (e^(rΔt) - d) / (u - d)
```

Build the terminal payoff, then backward-induct:

```cpp
double binomial_call(double S, double K, double r, double sigma, double T, int steps) {
    double dt = T / steps;
    double u  = std::exp(sigma * std::sqrt(dt));
    double d  = 1.0 / u;
    double p  = (std::exp(r * dt) - d) / (u - d);
    double df = std::exp(-r * dt);

    std::vector<double> V(steps + 1);
    for (int i = 0; i <= steps; ++i)
        V[i] = std::max(S * std::pow(u, i) * std::pow(d, steps - i) - K, 0.0);

    for (int t = steps - 1; t >= 0; --t)
        for (int i = 0; i <= t; ++i)
            V[i] = df * (p * V[i+1] + (1-p) * V[i]);

    return V[0];
}
```

For American options, at each node take `max(intrinsic, continuation)`.

---

## Step 5 — Put-Call Parity Check

A key sanity check: `C - P = S - K·e^(-rT)`

```cpp
double call = bsm_call(S, K, r, sigma, T);
double put  = bsm_put (S, K, r, sigma, T);
double parity_lhs = call - put;
double parity_rhs = S - K * std::exp(-r * T);
assert(std::abs(parity_lhs - parity_rhs) < 1e-9);
```

If this check fails, your put formula is wrong.

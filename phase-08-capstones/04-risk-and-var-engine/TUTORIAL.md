# Tutorial — Risk and VaR Engine

## Step 1 — Value at Risk Concepts

VaR(α, T) is the loss L such that the probability of exceeding L over T days is (1-α).

Example: VaR(95%, 1d) = $1M means there is a 5% chance of losing more than $1M tomorrow.

**Three methods:**

| Method | Pros | Cons |
|--------|------|------|
| Historical | No distribution assumption | Needs lots of history |
| Parametric | Fast, analytical | Assumes normality |
| Monte Carlo | Flexible | Computationally expensive |

---

## Step 2 — Historical Simulation VaR

```cpp
double historical_var(std::vector<double> returns, double confidence) {
    std::sort(returns.begin(), returns.end());
    // For 95% VaR, cut at the 5th percentile (worst 5% of days)
    int idx = static_cast<int>((1.0 - confidence) * returns.size());
    return -returns[idx];   // loss is expressed as positive
}
```

Expected Shortfall = mean of the tail beyond VaR:
```cpp
double expected_shortfall(std::vector<double> returns, double confidence) {
    std::sort(returns.begin(), returns.end());
    int cut = static_cast<int>((1.0 - confidence) * returns.size());
    double sum = 0.0;
    for (int i = 0; i < cut; ++i) sum += returns[i];
    return -sum / cut;
}
```

> **Docs:** https://en.cppreference.com/w/cpp/algorithm/sort

---

## Step 3 — Parametric VaR

Assuming normally distributed returns with mean μ and volatility σ:
```
VaR(α) = -(μ - z_α * σ)
```
where `z_α = N^(-1)(1-α)` is the z-score. For α=95%: z = 1.6449.

```cpp
double norm_quantile(double p) {
    // Beasley-Springer-Moro approximation (or use boost::math)
    // Simple approximation: z(95%) ≈ 1.6449
    static const double z95 = 1.6448536269514729;
    if (std::abs(p - 0.05) < 1e-9) return z95;
    // ... full inverse normal CDF
}

double parametric_var(double mean, double vol, double confidence) {
    double z = norm_quantile(1.0 - confidence);
    return -(mean - z * vol);
}
```

---

## Step 4 — Correlation Matrix

```cpp
std::vector<std::vector<double>>
correlation_matrix(const std::vector<std::vector<double>>& returns) {
    int n = returns.size();
    std::vector<std::vector<double>> corr(n, std::vector<double>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j <= i; ++j) {
            corr[i][j] = pearson(returns[i], returns[j]);
            corr[j][i] = corr[i][j];
        }
    }
    return corr;
}
```

Where `pearson(x, y) = cov(x,y) / (std(x) * std(y))`.

---

## Step 5 — Portfolio Beta

Beta measures systematic risk relative to the market:
```
β = cov(R_portfolio, R_market) / var(R_market)
```

```cpp
double portfolio_beta(const std::vector<double>& port_returns,
                       const std::vector<double>& market_returns) {
    double cov_pm = covariance(port_returns, market_returns);
    double var_m  = variance(market_returns);
    if (var_m < 1e-12) return 0.0;
    return cov_pm / var_m;
}
```

---

## Step 6 — HHI Concentration Index

The Herfindahl-Hirschman Index measures portfolio concentration:
```
HHI = Σ w_i²
```
HHI = 1/n for an equally-weighted portfolio (minimum), HHI = 1.0 for a single position.

```cpp
double hhi(const std::vector<double>& weights) {
    return std::inner_product(weights.begin(), weights.end(),
                               weights.begin(), 0.0);
}
```

> **Docs:** https://en.cppreference.com/w/cpp/algorithm/inner_product

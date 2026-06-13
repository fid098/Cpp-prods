# Tutorial — Template Statistics Library

## Step 1 — Function Templates

```cpp
template<typename T>
T mean(const std::vector<T>& data) {
    T sum = std::accumulate(data.begin(), data.end(), T{0});
    return sum / static_cast<T>(data.size());
}

// Used:
mean(std::vector<double>{1.0, 2.0, 3.0});  // 2.0 (T=double, deduced)
mean(std::vector<float>{1.0f, 2.0f});       // 1.5f (T=float)
```

The compiler **deduces** `T` from the argument type.

> **Docs:** https://en.cppreference.com/w/cpp/language/function_template

---

## Step 2 — Class Templates

```cpp
template<typename T>
class Stats {
public:
    static_assert(std::is_floating_point_v<T>,
                  "Stats requires a floating-point type");

    explicit Stats(const std::vector<T>& data) : data_{data} {}

    T mean()    const;
    T variance() const;
    T std_dev() const { return std::sqrt(variance()); }

private:
    std::vector<T> data_;
};
```

`static_assert(std::is_floating_point_v<T>, ...)` provides a clear error message if you try `Stats<int>`.

---

## Step 3 — Variance

```cpp
template<typename T>
T Stats<T>::variance() const {
    T m = mean();
    T sum_sq = T{0};
    for (const T& x : data_)
        sum_sq += (x - m) * (x - m);
    return sum_sq / static_cast<T>(data_.size() - 1);  // sample variance
}
```

---

## Step 4 — Skewness

```cpp
template<typename T>
T Stats<T>::skewness() const {
    T m  = mean();
    T sd = std_dev();
    T n  = static_cast<T>(data_.size());
    T sum_cubed = T{0};
    for (const T& x : data_)
        sum_cubed += std::pow((x - m) / sd, T{3});
    return sum_cubed / n;
}
```

---

## Step 5 — Two-series Correlation

```cpp
template<typename T>
T correlation(const std::vector<T>& x, const std::vector<T>& y) {
    Stats<T> sx(x), sy(y);
    T mx = sx.mean(), my = sy.mean();
    T cov = T{0};
    for (std::size_t i = 0; i < x.size(); ++i)
        cov += (x[i] - mx) * (y[i] - my);
    cov /= static_cast<T>(x.size() - 1);
    return cov / (sx.std_dev() * sy.std_dev());
}
```

---

## Common Pitfalls

| Mistake | Fix |
|---------|-----|
| `mean(data) / data.size()` integer division | Cast size to T: `/ static_cast<T>(data.size())` |
| Template definitions in `.cpp` file | Templates must be in headers (or explicit instantiation is needed) |
| Returning `T{}` without initialising | `T{0}` initialises to zero for all numeric types |

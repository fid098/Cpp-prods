#pragma once
#include <vector>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <type_traits>

template<typename T>
class Stats {
    static_assert(std::is_floating_point_v<T>,
                  "Stats<T> requires a floating-point type (float, double, long double)");
public:
    explicit Stats(const std::vector<T>& data) : data_{data} {
        if (data_.empty()) throw std::invalid_argument("Stats: empty data");
    }

    // Arithmetic mean
    T mean() const {
        T sum = std::accumulate(data_.begin(), data_.end(), T{0});
        return sum / static_cast<T>(data_.size());
    }

    // Sample variance (divided by n-1)
    T variance() const {
        if (data_.size() < 2) throw std::invalid_argument("variance requires >= 2 elements");
        T m = mean();
        T sum_sq = T{0};
        for (const T& x : data_) sum_sq += (x - m) * (x - m);
        return sum_sq / static_cast<T>(data_.size() - 1);
    }

    T std_dev()  const { return std::sqrt(variance()); }

    // Sample skewness
    T skewness() const {
        T m  = mean();
        T sd = std_dev();
        T n  = static_cast<T>(data_.size());
        T sum = T{0};
        for (const T& x : data_) sum += std::pow((x - m) / sd, T{3});
        return sum / n;
    }

    // Excess kurtosis (kurtosis - 3)
    T kurtosis() const {
        T m  = mean();
        T sd = std_dev();
        T n  = static_cast<T>(data_.size());
        T sum = T{0};
        for (const T& x : data_) sum += std::pow((x - m) / sd, T{4});
        return sum / n - T{3};
    }

    T min() const { return *std::min_element(data_.begin(), data_.end()); }
    T max() const { return *std::max_element(data_.begin(), data_.end()); }
    std::size_t size() const { return data_.size(); }

private:
    std::vector<T> data_;
};

// Pearson correlation coefficient between two series
template<typename T>
T correlation(const std::vector<T>& x, const std::vector<T>& y) {
    if (x.size() != y.size() || x.size() < 2)
        throw std::invalid_argument("correlation: mismatched or too-short series");
    Stats<T> sx{x}, sy{y};
    T mx = sx.mean(), my = sy.mean();
    T cov = T{0};
    for (std::size_t i = 0; i < x.size(); ++i) cov += (x[i] - mx) * (y[i] - my);
    cov /= static_cast<T>(x.size() - 1);
    return cov / (sx.std_dev() * sy.std_dev());
}

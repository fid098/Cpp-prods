#pragma once
#include "events.hpp"
#include <deque>
#include <vector>
#include <numeric>
#include <cmath>

class Strategy {
public:
    virtual ~Strategy() = default;
    virtual void on_bar(const BarEvent& bar, std::deque<Event>& queue) = 0;
};

// Simple Moving Average Crossover strategy.
class SMACrossover : public Strategy {
public:
    SMACrossover(int fast_period, int slow_period)
        : fast_{fast_period}, slow_{slow_period} {}

    void on_bar(const BarEvent& bar, std::deque<Event>& queue) override {
        prices_.push_back(bar.close);
        int n = static_cast<int>(prices_.size());
        if (n < slow_) return;

        double fast_ma = sma(n - fast_, fast_);
        double slow_ma = sma(n - slow_, slow_);

        if (fast_ma > slow_ma && !long_) {
            long_ = true;
            queue.push_back(SignalEvent{bar.symbol, +1});
        } else if (fast_ma < slow_ma && long_) {
            long_ = false;
            queue.push_back(SignalEvent{bar.symbol, -1});
        }
    }

private:
    int fast_, slow_;
    bool long_{false};
    std::vector<double> prices_;

    double sma(int start, int len) const {
        double sum = 0.0;
        for (int i = start; i < start + len; ++i) sum += prices_[i];
        return sum / len;
    }
};

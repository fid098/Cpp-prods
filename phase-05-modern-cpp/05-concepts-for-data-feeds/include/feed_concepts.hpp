#pragma once
#include <concepts>
#include <string>
#include <vector>
#include <utility>

// ── Concepts ──────────────────────────────────────────────────────────────────

template<typename T>
concept DataFeed = requires(const T feed) {
    { feed.price()  } -> std::convertible_to<double>;
    { feed.symbol() } -> std::convertible_to<std::string>;
} && requires(T feed) {
    { feed.next()   } -> std::same_as<bool>;
};

template<typename T>
concept TradingStrategy = requires(T strat, double price, int bar) {
    { strat.signal(price, bar) } -> std::same_as<int>;
    { strat.reset()            } -> std::same_as<void>;
};

// ── Concrete Implementations ──────────────────────────────────────────────────

class TickFeed {
public:
    TickFeed(std::string sym, std::vector<double> prices)
        : symbol_{std::move(sym)}, prices_{std::move(prices)}, idx_{0} {}

    bool        next()   { return idx_++ < static_cast<int>(prices_.size()); }
    double      price()  const { return idx_ > 0 ? prices_[idx_ - 1] : 0.0; }
    std::string symbol() const { return symbol_; }

private:
    std::string         symbol_;
    std::vector<double> prices_;
    int                 idx_{0};
};

static_assert(DataFeed<TickFeed>, "TickFeed must satisfy DataFeed");

class MomentumStrategy {
public:
    explicit MomentumStrategy(int lookback, double threshold)
        : lookback_{lookback}, threshold_{threshold} {}

    // Returns 1 (buy), -1 (sell), or 0 (hold)
    int signal(double price, int bar) {
        history_.push_back(price);
        if (bar < lookback_) return 0;
        double ret = (price - history_[bar - lookback_]) / history_[bar - lookback_];
        if (ret > threshold_) return 1;
        if (ret < -threshold_) return -1;
        return 0;
    }

    void reset() { history_.clear(); }

private:
    int                 lookback_;
    double              threshold_;
    std::vector<double> history_;
};

static_assert(TradingStrategy<MomentumStrategy>, "MomentumStrategy must satisfy TradingStrategy");

// ── Generic run function ──────────────────────────────────────────────────────

template<DataFeed Feed, TradingStrategy Strat>
int count_signals(Feed feed, Strat strategy) {
    int bar = 0, count = 0;
    while (feed.next()) {
        int sig = strategy.signal(feed.price(), bar++);
        if (sig != 0) ++count;
    }
    return count;
}

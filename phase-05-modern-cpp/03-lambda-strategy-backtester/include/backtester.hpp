#pragma once
#include <functional>
#include <vector>

// A strategy is a callable: given price series and current index, return a signal.
// Signal: 1 = BUY, -1 = SELL, 0 = HOLD
using Strategy = std::function<int(const std::vector<double>&, int)>;

// Built-in strategies ─────────────────────────────────────────────────────────

// Returns 1 if n-day return > threshold, -1 if < -threshold, else 0.
inline Strategy momentum_strategy(int lookback, double threshold) {
    // TODO: return a lambda that captures lookback and threshold
    (void)lookback; (void)threshold;
    return [](const std::vector<double>&, int) { return 0; };
}

// Returns 1 if price is above its n-day moving average, -1 if below.
inline Strategy mean_reversion_strategy(int window) {
    // TODO: return a lambda that computes the mean of the last `window` prices
    (void)window;
    return [](const std::vector<double>&, int) { return 0; };
}

// Combinators ────────────────────────────────────────────────────────────────

// Returns the signal only if both strategies agree; else 0.
inline Strategy and_strategy(Strategy s1, Strategy s2) {
    return [s1, s2](const std::vector<double>& p, int i) {
        auto sig1 = s1(p, i);
        auto sig2 = s2(p, i);
        return (sig1 == sig2) ? sig1 : 0;
    };
}

// Inverts the signals of a strategy.
inline Strategy invert(Strategy s) {
    return [s](const std::vector<double>& p, int i) { return -s(p, i); };
}

// Backtester ─────────────────────────────────────────────────────────────────

struct BacktestResult {
    double pnl{0.0};
    int    trades{0};
};

// Runs the strategy over prices, going long on BUY and flat on SELL.
// position_size is the number of shares per trade.
inline BacktestResult backtest(const std::vector<double>& prices,
                                const Strategy& strategy,
                                double position_size = 100.0) {
    BacktestResult result{};
    int position = 0;
    double entry_price = 0.0;

    for (int i = 1; i < static_cast<int>(prices.size()); ++i) {
        int sig = strategy(prices, i);

        if (sig == 1 && position == 0) {
            position    = 1;
            entry_price = prices[i];
        } else if (sig == -1 && position == 1) {
            result.pnl    += (prices[i] - entry_price) * position_size;
            result.trades += 1;
            position       = 0;
        }
    }
    return result;
}

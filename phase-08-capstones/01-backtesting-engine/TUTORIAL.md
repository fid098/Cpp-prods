# Tutorial — Backtesting Engine

## Step 1 — The Event Bus

The backbone of an event-driven backtest is a queue of typed events. `std::variant` holds any event type without inheritance:

```cpp
struct BarEvent   { std::string symbol; double open, high, low, close; long volume; };
struct SignalEvent { std::string symbol; int direction; };   // +1 BUY, -1 SELL
struct OrderEvent  { std::string symbol; int quantity; double price; };
struct FillEvent   { std::string symbol; int quantity; double fill_price; double commission; };

using Event = std::variant<BarEvent, SignalEvent, OrderEvent, FillEvent>;
```

Dispatching:
```cpp
void dispatch(const Event& e) {
    std::visit([&](auto&& ev) { handle(ev); }, e);
}
```

> **Docs:** https://en.cppreference.com/w/cpp/utility/variant/visit

---

## Step 2 — Strategy Interface

```cpp
class Strategy {
public:
    virtual ~Strategy() = default;
    virtual void on_bar(const BarEvent& bar, std::deque<Event>& queue) = 0;
};
```

A simple moving average crossover strategy:
```cpp
class SMACrossover : public Strategy {
    int fast_, slow_;
    std::vector<double> prices_;
public:
    SMACrossover(int fast, int slow) : fast_{fast}, slow_{slow} {}

    void on_bar(const BarEvent& bar, std::deque<Event>& queue) override {
        prices_.push_back(bar.close);
        if (static_cast<int>(prices_.size()) < slow_) return;

        double fast_ma = average(prices_.end() - fast_, prices_.end());
        double slow_ma = average(prices_.end() - slow_, prices_.end());

        if (fast_ma > slow_ma && !long_)
            queue.push_back(SignalEvent{bar.symbol, +1});
        else if (fast_ma < slow_ma && long_)
            queue.push_back(SignalEvent{bar.symbol, -1});
    }
private:
    bool long_{false};
    template<typename It>
    double average(It b, It e) { return std::accumulate(b, e, 0.0) / std::distance(b, e); }
};
```

---

## Step 3 — Broker Simulation

```cpp
class SimBroker {
    double commission_pct_{0.001};  // 10 bps per side
public:
    void on_order(const OrderEvent& o, std::deque<Event>& queue) {
        double commission = o.price * o.quantity * commission_pct_;
        queue.push_back(FillEvent{o.symbol, o.quantity, o.price, commission});
    }
};
```

For a more realistic simulation, add slippage:
```cpp
double fill_price = o.price * (1.0 + 0.0001 * o.quantity / avg_daily_volume);
```

---

## Step 4 — Portfolio Tracking

```cpp
class Portfolio {
    double cash_;
    double equity_;
    std::unordered_map<std::string, int> positions_;
    std::vector<double> equity_curve_;
public:
    void on_fill(const FillEvent& fill) {
        positions_[fill.symbol] += fill.quantity;
        cash_ -= fill.quantity * fill.fill_price + fill.commission;
    }
    void on_bar(const BarEvent& bar) {
        // mark-to-market: update equity with current price
        double mkt_value = positions_[bar.symbol] * bar.close;
        equity_ = cash_ + mkt_value;
        equity_curve_.push_back(equity_);
    }
};
```

---

## Step 5 — Performance Metrics

```cpp
double sharpe(const std::vector<double>& equity_curve) {
    std::vector<double> rets;
    for (std::size_t i = 1; i < equity_curve.size(); ++i)
        rets.push_back((equity_curve[i] - equity_curve[i-1]) / equity_curve[i-1]);
    double mean = std::accumulate(rets.begin(), rets.end(), 0.0) / rets.size();
    double var  = 0.0;
    for (double r : rets) var += (r - mean) * (r - mean);
    var /= rets.size();
    return mean / std::sqrt(var) * std::sqrt(252.0);
}

double max_drawdown(const std::vector<double>& equity_curve) {
    double peak = equity_curve[0], max_dd = 0.0;
    for (double eq : equity_curve) {
        if (eq > peak) peak = eq;
        max_dd = std::max(max_dd, (peak - eq) / peak);
    }
    return max_dd;
}
```

> **Docs:** https://en.cppreference.com/w/cpp/algorithm/accumulate

---

## Step 6 — Main Loop

```cpp
std::deque<Event> queue;
BarFeed feed{"data/sample_ohlcv.csv"};
SMACrossover strategy{5, 20};
SimBroker broker;
Portfolio portfolio{100'000.0};

while (feed.has_next()) {
    queue.push_back(feed.next());
    while (!queue.empty()) {
        Event e = queue.front(); queue.pop_front();
        std::visit([&](auto&& ev) {
            using T = std::decay_t<decltype(ev)>;
            if constexpr (std::is_same_v<T, BarEvent>) {
                strategy.on_bar(ev, queue);
                portfolio.on_bar(ev);
            } else if constexpr (std::is_same_v<T, SignalEvent>) {
                queue.push_back(OrderEvent{ev.symbol, 100 * ev.direction, feed.last_close()});
            } else if constexpr (std::is_same_v<T, OrderEvent>) {
                broker.on_order(ev, queue);
            } else if constexpr (std::is_same_v<T, FillEvent>) {
                portfolio.on_fill(ev);
            }
        }, e);
    }
}
```

#pragma once
#include <atomic>
#include <string>
#include <string_view>
#include <array>
#include <stdexcept>

struct InstrumentData {
    std::atomic<double> bid{0.0};
    std::atomic<double> ask{0.0};
    std::atomic<double> last{0.0};
    std::atomic<double> prev_last{0.0};
    std::atomic<bool>   valid{false};

    InstrumentData() = default;
    InstrumentData(const InstrumentData&) = delete;
    InstrumentData& operator=(const InstrumentData&) = delete;
};

struct Snapshot {
    std::string symbol;
    double bid{0}, ask{0}, last{0}, prev_last{0};
    bool   valid{false};
    double spread_bps() const {
        double mid = (bid + ask) / 2.0;
        return (mid > 0.0) ? (ask - bid) / mid * 10000.0 : 0.0;
    }
};

class PriceStore {
public:
    static constexpr int MAX_INSTRUMENTS = 16;

    bool register_instrument(std::string_view symbol, double initial_price) {
        if (count_ >= MAX_INSTRUMENTS) return false;
        int idx = count_++;
        symbols_[idx] = std::string{symbol};
        auto& d = data_[idx];
        double spread = initial_price * 0.0002;   // 2 bps
        d.bid.store(initial_price - spread, std::memory_order_relaxed);
        d.ask.store(initial_price + spread, std::memory_order_relaxed);
        d.last.store(initial_price,         std::memory_order_relaxed);
        d.prev_last.store(initial_price,    std::memory_order_relaxed);
        d.valid.store(true,                 std::memory_order_release);
        return true;
    }

    void update(int idx, double bid, double ask, double last) {
        auto& d = data_[idx];
        d.prev_last.store(d.last.load(std::memory_order_relaxed), std::memory_order_relaxed);
        d.bid.store(bid,  std::memory_order_relaxed);
        d.ask.store(ask,  std::memory_order_relaxed);
        d.last.store(last, std::memory_order_release);
    }

    Snapshot snapshot(int idx) const {
        const auto& d = data_[idx];
        if (!d.valid.load(std::memory_order_acquire)) return {};
        Snapshot s;
        s.symbol    = symbols_[idx];
        s.bid       = d.bid.load(std::memory_order_acquire);
        s.ask       = d.ask.load(std::memory_order_acquire);
        s.last      = d.last.load(std::memory_order_acquire);
        s.prev_last = d.prev_last.load(std::memory_order_acquire);
        s.valid     = true;
        return s;
    }

    int count() const { return count_; }

private:
    int         count_{0};
    std::string symbols_[MAX_INSTRUMENTS]{};
    InstrumentData data_[MAX_INSTRUMENTS];
};

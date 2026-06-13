#pragma once
#include <atomic>
#include <string>
#include <string_view>

// ---------------------------------------------------------------------------
// PriceTick — holds the latest bid/ask/last for one symbol.
// Atomics cannot be copied, so this struct is move-only (actually not even
// moveable by default — we store it in a fixed array, never move it).
// ---------------------------------------------------------------------------
struct PriceTick {
    std::atomic<double> bid{0.0};
    std::atomic<double> ask{0.0};
    std::atomic<double> last{0.0};
    std::atomic<bool>   valid{false};

    PriceTick() = default;
    PriceTick(const PriceTick&) = delete;
    PriceTick& operator=(const PriceTick&) = delete;
};

// ---------------------------------------------------------------------------
// MarketDataCache — lock-free cache for up to MAX_SYMBOLS instruments.
//
// Thread safety contract:
//   - update() may be called from any thread concurrently.
//   - read()   may be called from any thread concurrently with update().
//   - Symbol registration (the first call to update() for a new symbol) is
//     NOT thread-safe; register all symbols before starting publisher threads.
// ---------------------------------------------------------------------------
class MarketDataCache {
public:
    static constexpr int MAX_SYMBOLS = 16;

    struct Snapshot {
        double bid{0.0};
        double ask{0.0};
        double last{0.0};
        bool   valid{false};
    };

    // Register or update a symbol's prices.
    // Returns false if the cache is full and the symbol is new.
    inline bool update(std::string_view symbol, double bid, double ask, double last);

    // Read the latest snapshot for a symbol.
    // Returns Snapshot{valid=false} if the symbol is not found.
    inline Snapshot read(std::string_view symbol) const;

    // Total number of successful update() calls.
    inline long long update_count() const {
        return count_.load(std::memory_order_relaxed);
    }

    // Number of registered symbols.
    int size() const { return size_; }

private:
    std::string  symbols_[MAX_SYMBOLS]{};
    PriceTick    ticks_[MAX_SYMBOLS];
    int          size_{0};
    std::atomic<long long> count_{0};

    int find_index(std::string_view symbol) const {
        for (int i = 0; i < size_; ++i)
            if (symbols_[i] == symbol) return i;
        return -1;
    }
};

inline bool MarketDataCache::update(std::string_view symbol,
                                    double bid, double ask, double last) {
    int idx = find_index(symbol);
    if (idx < 0) {
        // Registration: caller must ensure this is single-threaded
        if (size_ >= MAX_SYMBOLS) return false;
        idx = size_;
        symbols_[idx] = std::string{symbol};
        ++size_;
    }

    // TODO: store bid, ask, last into ticks_[idx] using memory_order_release
    // TODO: set ticks_[idx].valid = true using memory_order_release
    // TODO: increment count_ using memory_order_relaxed

    (void)bid; (void)ask; (void)last; // suppress unused warnings until TODO is done
    return true;
}

inline MarketDataCache::Snapshot
MarketDataCache::read(std::string_view symbol) const {
    int idx = find_index(symbol);
    if (idx < 0) return {};

    Snapshot s;
    // TODO: load bid, ask, last from ticks_[idx] using memory_order_acquire
    // TODO: load valid from ticks_[idx] using memory_order_acquire
    return s;
}

#pragma once
#include <algorithm>
#include <utility>
#include <stdexcept>

struct Tick {
    double price{0.0};
    int    volume{0};
    long long timestamp{0};
};

class MarketDataFeed {
public:
    explicit MarketDataFeed(int capacity)
        : data_{new Tick[capacity]{}}, size_{0}, capacity_{capacity} {}

    // TODO: Destructor — delete[] data_
    ~MarketDataFeed();

    // TODO: Copy constructor — deep copy
    MarketDataFeed(const MarketDataFeed& other);

    // TODO: Copy assignment — deep copy with self-assign guard
    MarketDataFeed& operator=(const MarketDataFeed& other);

    // TODO: Move constructor — steal data_, null out other
    MarketDataFeed(MarketDataFeed&& other) noexcept;

    // TODO: Move assignment — steal with self-assign guard
    MarketDataFeed& operator=(MarketDataFeed&& other) noexcept;

    bool push(Tick t) {
        if (size_ >= capacity_) return false;
        data_[size_++] = t;
        return true;
    }

    int    size()     const { return size_; }
    int    capacity() const { return capacity_; }
    bool   empty()    const { return size_ == 0; }

    const Tick& operator[](int i) const { return data_[i]; }

private:
    Tick* data_{nullptr};
    int   size_{0};
    int   capacity_{0};
};

// ── Implementations (TODO stubs) ─────────────────────────────────────────────

inline MarketDataFeed::~MarketDataFeed() {
    // TODO: delete[] data_;
}

inline MarketDataFeed::MarketDataFeed(const MarketDataFeed& other)
    : data_{nullptr}, size_{0}, capacity_{0} {
    // TODO: allocate new Tick[other.capacity_], copy, set size_ and capacity_
    (void)other;
}

inline MarketDataFeed& MarketDataFeed::operator=(const MarketDataFeed& other) {
    // TODO: self-assign guard, delete old, deep copy
    (void)other;
    return *this;
}

inline MarketDataFeed::MarketDataFeed(MarketDataFeed&& other) noexcept
    : data_{nullptr}, size_{0}, capacity_{0} {
    // TODO: steal other.data_, null out other
    (void)other;
}

inline MarketDataFeed& MarketDataFeed::operator=(MarketDataFeed&& other) noexcept {
    // TODO: self-assign guard, delete old, steal
    (void)other;
    return *this;
}

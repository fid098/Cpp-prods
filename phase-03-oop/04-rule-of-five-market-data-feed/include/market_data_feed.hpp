#pragma once
#include <algorithm>
#include <stdexcept>
#include <utility>

struct Tick {
    double    price{0.0};
    int       volume{0};
    long long timestamp{0};
};

// ─────────────────────────────────────────────────────────────────────────────
// FeedStats — instrumentation so tests (and main) can SEE which special
// member functions ran, and prove that every allocation was freed.
// `static inline` members (C++17) are defined right here in the header,
// shared by every MarketDataFeed in the program.
// ─────────────────────────────────────────────────────────────────────────────
struct FeedStats {
    static inline int allocations = 0;   // successful new[]
    static inline int frees       = 0;   // delete[] of a non-null buffer
    static inline int copies      = 0;   // copy ctor + copy assignment
    static inline int moves       = 0;   // move ctor + move assignment

    static void reset() { allocations = frees = copies = moves = 0; }
    static int  live_buffers() { return allocations - frees; }
};

// ─────────────────────────────────────────────────────────────────────────────
// MarketDataFeed — OWNS a heap buffer of Ticks through a raw pointer.
//
// Owning a raw resource means the compiler-generated special members are
// WRONG: the default copy would copy the pointer (shallow → two owners →
// double delete), and the default destructor would never free it (leak).
// So we write all five. That's the Rule of Five.
//
// Invariant (true after every public operation, including after being moved
// from):  0 <= size_ <= capacity_,  and  data_ == nullptr  iff  capacity_ == 0
// ─────────────────────────────────────────────────────────────────────────────
class MarketDataFeed {
public:
    explicit MarketDataFeed(int capacity)
        : data_{allocate(capacity)}, size_{0}, capacity_{capacity} {}

    ~MarketDataFeed();                                        // 1. destructor
    MarketDataFeed(const MarketDataFeed& other);              // 2. copy ctor
    MarketDataFeed& operator=(const MarketDataFeed& other);   // 3. copy assign
    MarketDataFeed(MarketDataFeed&& other) noexcept;          // 4. move ctor
    MarketDataFeed& operator=(MarketDataFeed&& other) noexcept; // 5. move assign

    bool push(Tick t) {
        if (size_ >= capacity_) return false;
        data_[size_++] = t;
        return true;
    }

    int  size()     const { return size_; }
    int  capacity() const { return capacity_; }
    bool empty()    const { return size_ == 0; }

    // Unchecked access (fast, like std::vector::operator[]) — UB if out of range.
    const Tick& operator[](int i) const { return data_[i]; }
    Tick&       operator[](int i)       { return data_[i]; }

    // Checked access (like std::vector::at) — throws instead of UB.
    const Tick& at(int i) const {
        if (i < 0 || i >= size_) throw std::out_of_range{"MarketDataFeed::at"};
        return data_[i];
    }

    // Exposed only so tests/demo can prove two feeds use different buffers.
    const Tick* data() const { return data_; }

private:
    // One place that does `new[]`, one place that does `delete[]` — easier
    // to reason about and to instrument.
    static Tick* allocate(int capacity) {
        if (capacity < 0) throw std::invalid_argument{"capacity must be >= 0"};
        if (capacity == 0) return nullptr;
        Tick* p = new Tick[capacity]{};   // may throw std::bad_alloc
        ++FeedStats::allocations;
        return p;
    }
    static void release(Tick* p) noexcept {
        if (p) ++FeedStats::frees;
        delete[] p;                       // delete[] nullptr is a safe no-op
    }

    Tick* data_{nullptr};
    int   size_{0};
    int   capacity_{0};
};

// ── 1. Destructor ────────────────────────────────────────────────────────────
// Runs automatically when the object's lifetime ends (scope exit, delete,
// container erase…). Destructors are implicitly noexcept — never throw here.
inline MarketDataFeed::~MarketDataFeed() {
    release(data_);
}

// ── 2. Copy constructor — DEEP copy ──────────────────────────────────────────
// `*this` is brand new; there's no old buffer to free. Allocate our own
// buffer and copy the elements (not the pointer!) into it.
// If allocate() throws, the object was never constructed, so no destructor
// runs and nothing leaks.
inline MarketDataFeed::MarketDataFeed(const MarketDataFeed& other)
    : data_{allocate(other.capacity_)},
      size_{other.size_},
      capacity_{other.capacity_} {
    std::copy(other.data_, other.data_ + other.size_, data_);
    ++FeedStats::copies;
}

// ── 3. Copy assignment — deep copy, STRONG exception guarantee ──────────────
// `*this` already owns a buffer. Order matters:
//   1. allocate + copy into a NEW buffer   (may throw → *this untouched)
//   2. only then free the OLD buffer       (can't throw)
//   3. commit the new pointer/size/capacity
// The naive "delete[] old; data_ = new …" leaves data_ dangling if new
// throws, and the destructor would later double-free it.
inline MarketDataFeed& MarketDataFeed::operator=(const MarketDataFeed& other) {
    if (this == &other) return *this;   // self-assignment: nothing to do

    Tick* fresh = allocate(other.capacity_);                 // step 1
    std::copy(other.data_, other.data_ + other.size_, fresh);

    release(data_);                                          // step 2
    data_     = fresh;                                       // step 3
    size_     = other.size_;
    capacity_ = other.capacity_;
    ++FeedStats::copies;
    return *this;                       // enables a = b = c
}

// ── 4. Move constructor — STEAL, don't copy ──────────────────────────────────
// `other` is an rvalue (a temporary, or something you std::move'd) — the
// caller promised not to rely on its value. So take its pointer: O(1), no
// allocation, can't fail → noexcept.
// std::exchange(x, v) sets x = v and returns x's OLD value — steal + reset in
// one expression. `other` is left valid-but-empty, so its destructor is safe.
inline MarketDataFeed::MarketDataFeed(MarketDataFeed&& other) noexcept
    : data_{std::exchange(other.data_, nullptr)},
      size_{std::exchange(other.size_, 0)},
      capacity_{std::exchange(other.capacity_, 0)} {
    ++FeedStats::moves;
}

// ── 5. Move assignment ───────────────────────────────────────────────────────
// Like the move ctor, but *this already owns a buffer that must be freed
// first. The self-check matters here: `a = std::move(a)` without it would
// free our own buffer and then "steal" the freed pointer.
inline MarketDataFeed& MarketDataFeed::operator=(MarketDataFeed&& other) noexcept {
    if (this == &other) return *this;

    release(data_);
    data_     = std::exchange(other.data_, nullptr);
    size_     = std::exchange(other.size_, 0);
    capacity_ = std::exchange(other.capacity_, 0);
    ++FeedStats::moves;
    return *this;
}

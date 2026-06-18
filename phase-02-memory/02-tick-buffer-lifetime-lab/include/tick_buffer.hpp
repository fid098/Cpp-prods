#pragma once
#include <array>
#include <cstddef>

struct Tick
{
    char symbol[8]{};
    double price{0.0};
    int volume{0};
    long long timestamp{0};
};

// Populates n ticks with synthetic data (price = 100 + i*0.01, volume = 100+i)
inline void fill_buffer(Tick *buf, int n)
{
    // Done: loop i in [0,n), set buf[i].price, buf[i].volume, buf[i].timestamp
    for (int i{}; i < n; i++)
    {
        buf[i].price = 100 + i * 0.01;
        buf[i].volume = 100 + i;
        buf[i].timestamp = i;
    }
}

// Returns the sum of all volumes in buf[0..n-1] using pointer arithmetic
inline long long sum_volume(const Tick *buf, int n)
{
    // Done: for (const Tick* p = buf; p < buf + n; ++p) total += p->volume
    long long sum{0};
    for (const Tick* p = buf; p < buf + n; p++)
    {
        sum += p->volume;
    }
    return sum;
}

// Returns a heap-allocated array of n Ticks. Caller must delete[].
inline Tick *make_heap_buffer(int n)
{
    // Done: return new Tick[n]{}
    if (n > 0)
    {
        return new Tick[n]{};
    }
    else
    {
        return nullptr;
    }
}

// Returns a stack-allocated buffer of 512 ticks (returned by value).
inline std::array<Tick, 512> make_stack_buffer()
{
    std::array<Tick, 512> buf{};
    return buf;
}

#pragma once // to avoid padding error

struct OrderEntry
{
    double price{0.0};
    int quantity{0};
    char side{'\0'}; // 'B' = bid, 'A' = ask
};

// Sums the quantities of n entries starting at the pointer p.
// Uses pointer arithmetic: for (const OrderEntry* it = p; it < p + n; ++it)
inline int total_quantity(const OrderEntry *p, int n)
{
    // Done: iterate with pointer arithmetic, sum p->quantity
    int total{0};
    for (const OrderEntry *entry = p; entry < p + n; entry++)
    {
        total += entry->quantity;
    }
    return total;
}

// Returns a pointer to the entry with the highest price, or nullptr if n == 0.
inline const OrderEntry *best_price(const OrderEntry *p, int n)
{
    // Done: iterate, track pointer to max price entry
    const OrderEntry *highest_pointer{p};
    if (n == 0)
    {
        return nullptr;
    }
    for (const OrderEntry *entry = p; entry < p + n; entry++)
    {
        if (entry->price > highest_pointer->price)
        {
            highest_pointer = entry;
        }
    }
    return highest_pointer;
}

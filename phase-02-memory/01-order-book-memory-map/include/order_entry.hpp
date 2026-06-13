#pragma once

struct OrderEntry {
    double price{0.0};
    int    quantity{0};
    char   side{'\0'};  // 'B' = bid, 'A' = ask
};

// Sums the quantities of n entries starting at the pointer p.
// Uses pointer arithmetic: for (const OrderEntry* it = p; it < p + n; ++it)
inline int total_quantity(const OrderEntry* p, int n) {
    // TODO: iterate with pointer arithmetic, sum p->quantity
    (void)p; (void)n;
    return 0;
}

// Returns a pointer to the entry with the highest price, or nullptr if n == 0.
inline const OrderEntry* best_price(const OrderEntry* p, int n) {
    // TODO: iterate, track pointer to max price entry
    (void)p; (void)n;
    return nullptr;
}

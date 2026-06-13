#pragma once
#include <cstring>
#include <string>

struct Position {
    char   symbol[8]{};
    int    quantity{0};
    double avg_cost{0.0};
};

// A simple heap-managed cache of positions.
// THIS CODE CONTAINS INTENTIONAL BUGS — find and fix them all.
struct PortfolioCache {
    Position* positions{nullptr};
    int       count{0};
    int       capacity{0};

    // Allocates space for `cap` positions.
    void init(int cap) {
        capacity  = cap;
        count     = 0;
        positions = new Position[cap]{};
    }

    // Adds a position. Returns false if cache is full.
    bool add(const char* sym, int qty, double cost) {
        if (count >= capacity) return false;
        std::strncpy(positions[count].symbol, sym, 7);
        positions[count].quantity = qty;
        positions[count].avg_cost = cost;
        ++count;
        return true;
    }

    // Returns a pointer to the position with the given symbol, or nullptr.
    const Position* find(const char* sym) const {
        for (int i = 0; i < count; ++i) {
            if (std::strncmp(positions[i].symbol, sym, 7) == 0)
                return &positions[i];
        }
        return nullptr;
    }

    // Computes total portfolio value (sum of qty * avg_cost).
    double total_value() const {
        double total = 0.0;
        for (int i = 0; i < count; ++i)
            total += positions[i].quantity * positions[i].avg_cost;
        return total;
    }

    // BUG 1: This function leaks — it creates a temp buffer but never frees it.
    void refresh_metadata() {
        Position* temp = new Position[capacity]{};
        // copies current positions into temp for "validation"
        for (int i = 0; i < count; ++i) temp[i] = positions[i];
        // TODO (BUG 1): temp is never deleted — add delete[] temp; here
    }

    // BUG 2: double-free — clear() is called twice in main.cpp.
    // Fix: set positions = nullptr after delete[].
    void clear() {
        delete[] positions;
        // BUG 2: add:  positions = nullptr;
        count    = 0;
        capacity = 0;
    }
};

// BUG 3: Returns a pointer to a local variable (dangling pointer after return).
// Fix: either return by value or allocate on heap.
inline Position* make_default_position() {
    Position local{"DEFAULT", 0, 0.0};
    return &local;  // BUG 3: local is destroyed after this return
}

// BUG 4: Uses the result of make_default_position without checking staleness.
// (After you fix Bug 3, this function will work correctly.)
inline std::string describe_default() {
    const Position* p = make_default_position();
    return std::string(p->symbol);  // BUG 4: p may be dangling
}

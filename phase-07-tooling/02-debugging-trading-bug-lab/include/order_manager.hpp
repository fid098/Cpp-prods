#pragma once
#include <unordered_map>
#include <stdexcept>
#include <string>

// ---------------------------------------------------------------------------
// BUG 3: Logic error in fill_order()
//   The fill condition uses "<" instead of ">=", so orders that exactly match
//   available quantity are never filled.
//
// BUG 4: Null dereference in get_order_price()
//   When an order ID is not found, find() returns end() and dereferencing
//   it is undefined behaviour (likely a crash).
// ---------------------------------------------------------------------------

struct Order {
    int    id;
    int    quantity;
    double price;
    bool   filled{false};
};

class OrderManager {
public:
    void add_order(int id, int quantity, double price) {
        orders_[id] = Order{id, quantity, price};
    }

    // Returns true and marks order as filled if available >= requested.
    // BUG 3: condition is inverted — should be available >= requested
    bool fill_order(int id, int available) {
        auto it = orders_.find(id);
        if (it == orders_.end()) return false;
        // BUG 3: should be available >= it->second.quantity
        if (available < it->second.quantity) {
            it->second.filled = true;
            return true;
        }
        return false;
    }

    // Returns the price for a given order ID.
    // BUG 4: does not check whether the order exists before dereferencing
    double get_order_price(int id) const {
        auto it = orders_.find(id);
        // BUG 4: missing null check: if (it == orders_.end()) throw ...
        return it->second.price;
    }

    bool is_filled(int id) const {
        auto it = orders_.find(id);
        if (it == orders_.end()) return false;
        return it->second.filled;
    }

private:
    std::unordered_map<int, Order> orders_;
};

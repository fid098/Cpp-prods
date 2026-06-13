#pragma once
#include <string>

// Represents one open stock position.
struct Position {
    std::string symbol;
    int         quantity{0};
    double      avg_cost{0.0};
    double      current_price{0.0};
};

// Returns the new weighted-average cost after adding new_qty shares at price.
// Formula: (old_qty * old_avg + new_qty * price) / (old_qty + new_qty)
// Precondition: (old_qty + new_qty) > 0
inline double compute_avg_cost(int old_qty, double old_avg,
                                int new_qty, double price) {
    // TODO: implement
    (void)old_qty; (void)old_avg; (void)new_qty; (void)price;
    return 0.0;
}

// Returns the realized PnL from selling qty_sold shares of pos at sell_price.
// Formula: (sell_price - pos.avg_cost) * qty_sold
// Precondition: qty_sold <= pos.quantity
inline double compute_realized_pnl(const Position& pos,
                                    int qty_sold, double sell_price) {
    // TODO: implement
    (void)pos; (void)qty_sold; (void)sell_price;
    return 0.0;
}

// Returns the current unrealized PnL for an open position.
// Formula: (pos.current_price - pos.avg_cost) * pos.quantity
inline double compute_unrealized_pnl(const Position& pos) {
    // TODO: implement
    (void)pos;
    return 0.0;
}

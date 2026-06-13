#pragma once
#include <string>

// Represents one open stock position.
struct Position
{
    std::string symbol;
    int quantity{0};
    double avg_cost{0.0};
    double current_price{0.0};
};

// Returns the new weighted-average cost after adding new_qty shares at price.
// Formula: (old_qty * old_avg + new_qty * price) / (old_qty + new_qty)
// Precondition: (old_qty + new_qty) > 0
inline double compute_avg_cost(int old_qty, double old_avg,
                               int new_qty, double price)
{
    // DONE
    double avg_cost = 0.0;
    double cast_old_qty = static_cast<double>(old_qty);
    double cast_new_qty = static_cast<double>(new_qty);
    if ((cast_old_qty + cast_new_qty) > 0)
    {
        avg_cost = ((cast_old_qty * old_avg) + (cast_new_qty * price)) / (cast_old_qty + cast_new_qty);
    }

    return avg_cost;
}

// Returns the realized PnL from selling qty_sold shares of pos at sell_price.
// Formula: (sell_price - pos.avg_cost) * qty_sold
// Precondition: qty_sold <= pos.quantity
inline double compute_realized_pnl(const Position &pos,
                                   int qty_sold, double sell_price)
{
    // DONE
    double cast_qt_sold = static_cast<double>(qty_sold);
    double realised_pnl = (sell_price - pos.avg_cost) * cast_qt_sold;
    return realised_pnl;
}

// Returns the current unrealized PnL for an open position.
// Formula: (pos.current_price - pos.avg_cost) * pos.quantity
inline double compute_unrealized_pnl(const Position &pos)
{
    // DONE
    double unrealised_pnl = (pos.current_price - pos.avg_cost) * static_cast<double>(pos.quantity);
    return unrealised_pnl;
}

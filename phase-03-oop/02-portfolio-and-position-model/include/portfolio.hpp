#pragma once
#include "position.hpp"
#include <vector>
#include <stdexcept>

class Portfolio
{
public:
    // DONE: implement add_position — push_back into positions_
    void add_position(const Position &pos)
    {
        positions_.push_back(pos);
    }

    // DONE: implement total_value — sum p.value() for all positions
    double total_value() const
    {
        double total = 0.0;
        for (const auto &pos : positions_)
        {
            total += pos.value();
        }
        return total;
    }

    // Returns the weight (fraction of total value) of the given symbol.
    // Returns 0.0 if symbol not found or total_value == 0.
    // DONE: implement
    double weight(const std::string &symbol) const
    {
        double total = total_value();
        if (total == 0.0)
            return 0.0;
        for (const auto &pos : positions_)
        {
            if (pos.symbol() == symbol)
            {
                return pos.value() / total;
            }
        }
        return 0.0;
    }

    int size() const { return static_cast<int>(positions_.size()); }

    const std::vector<Position> &positions() const { return positions_; }

    // DONE: implement print_report — print symbol, qty, price, value, weight
    void print_report() const
    {
        for (const auto &pos : positions_)
        {
            std::cout << "Symbol: " << pos.symbol()
                      << ", Qty: " << pos.qty()
                      << ", Price: " << pos.price()
                      << ", Value: " << pos.value()
                      << ", Weight: " << weight(pos.symbol()) << " \n";
        }
    }

    // Explicit copy constructor (practice — compiler-generated would be identical)
    Portfolio(const Portfolio &other) : positions_{other.positions_} {}
    Portfolio &operator=(const Portfolio &other)
    {
        if (this != &other)
            positions_ = other.positions_;
        return *this;
    }
    Portfolio() = default;

private:
    std::vector<Position> positions_;
};

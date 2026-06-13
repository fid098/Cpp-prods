#pragma once
#include "position.hpp"
#include <vector>
#include <stdexcept>

class Portfolio {
public:
    // TODO: implement add_position — push_back into positions_
    void add_position(const Position& pos);

    // TODO: implement total_value — sum p.value() for all positions
    double total_value() const;

    // Returns the weight (fraction of total value) of the given symbol.
    // Returns 0.0 if symbol not found or total_value == 0.
    // TODO: implement
    double weight(const std::string& symbol) const;

    int size() const { return static_cast<int>(positions_.size()); }

    const std::vector<Position>& positions() const { return positions_; }

    // TODO: implement print_report — print symbol, qty, price, value, weight
    void print_report() const;

    // Explicit copy constructor (practice — compiler-generated would be identical)
    Portfolio(const Portfolio& other) : positions_{other.positions_} {}
    Portfolio& operator=(const Portfolio& other) {
        if (this != &other) positions_ = other.positions_;
        return *this;
    }
    Portfolio() = default;

private:
    std::vector<Position> positions_;
};

inline void Portfolio::add_position(const Position& pos) {
    // TODO
    (void)pos;
}

inline double Portfolio::total_value() const {
    // TODO
    return 0.0;
}

inline double Portfolio::weight(const std::string& symbol) const {
    // TODO
    (void)symbol;
    return 0.0;
}

inline void Portfolio::print_report() const {
    // TODO: print each position + weight
}

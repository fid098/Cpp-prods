#pragma once
#include "events.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

// Reads a CSV with header: symbol,open,high,low,close,volume
// Returns BarEvents one at a time.
class BarFeed {
public:
    explicit BarFeed(const std::string& path) {
        std::ifstream f{path};
        if (!f.is_open()) throw std::runtime_error("cannot open: " + path);
        std::string line;
        std::getline(f, line); // skip header
        while (std::getline(f, line)) {
            if (line.empty()) continue;
            std::istringstream ss{line};
            std::string tok;
            BarEvent bar;
            std::getline(ss, bar.symbol, ',');
            std::getline(ss, tok, ','); bar.open   = std::stod(tok);
            std::getline(ss, tok, ','); bar.high   = std::stod(tok);
            std::getline(ss, tok, ','); bar.low    = std::stod(tok);
            std::getline(ss, tok, ','); bar.close  = std::stod(tok);
            std::getline(ss, tok, ','); bar.volume = std::stol(tok);
            bars_.push_back(bar);
        }
    }

    // In-memory constructor for tests — avoids file I/O
    explicit BarFeed(std::vector<BarEvent> bars) : bars_{std::move(bars)} {}

    bool    has_next() const  { return idx_ < static_cast<int>(bars_.size()); }
    BarEvent next()           { return bars_[idx_++]; }
    double   last_close()     const { return idx_ > 0 ? bars_[idx_-1].close : 0.0; }
    int      total_bars()     const { return static_cast<int>(bars_.size()); }

private:
    std::vector<BarEvent> bars_;
    int idx_{0};
};

#pragma once
#include <string>

struct TradeTicket {
    std::string symbol;
    std::string side;    // "BUY" or "SELL"
    int         qty{0};
    std::string type;    // "MARKET" or "LIMIT"
    double      price{0.0};
};

struct ValidationResult {
    bool        accepted{false};
    std::string reason;
};

// Individual rule validators ─────────────────────────────────────────────────

// Symbol must be 1–5 non-whitespace characters
inline bool is_valid_symbol(const std::string& symbol) {
    // TODO: !symbol.empty() && symbol.length() <= 5
    (void)symbol;
    return false;
}

// Side must be exactly "BUY" or "SELL"
inline bool is_valid_side(const std::string& side) {
    // TODO: side == "BUY" || side == "SELL"
    (void)side;
    return false;
}

// Quantity must be strictly positive (int overload)
inline bool is_valid_qty(int qty) {
    // TODO: qty > 0
    (void)qty;
    return false;
}

// Quantity must be strictly positive (double overload — for CSV-parsed data)
inline bool is_valid_qty(double qty) {
    // TODO: qty > 0.0
    (void)qty;
    return false;
}

// Type must be "MARKET" or "LIMIT"
inline bool is_valid_type(const std::string& type) {
    // TODO
    (void)type;
    return false;
}

// LIMIT orders require price > 0; MARKET orders are always price-valid
inline bool is_valid_price(const std::string& type, double price) {
    // TODO: if type == "LIMIT" return price > 0.0, else return true
    (void)type; (void)price;
    return false;
}

// Master validator — runs all rules in order ─────────────────────────────────
inline ValidationResult validate(const TradeTicket& t) {
    // TODO: call each rule; return {false, reason} on first failure
    //       return {true, ""} if all rules pass
    (void)t;
    return {false, "not implemented"};
}

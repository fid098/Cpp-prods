#pragma once
#include <string>

struct TradeTicket
{
    std::string symbol;
    std::string side; // "BUY" or "SELL"
    int qty{0};
    std::string type; // "MARKET" or "LIMIT"
    double price{0.0};
};

struct ValidationResult
{
    bool accepted{false};
    std::string reason;
};

// Individual rule validators ─────────────────────────────────────────────────

// Symbol must be 1–5 non-whitespace characters
inline bool is_valid_symbol(const std::string &symbol)
{
    // Done: !symbol.empty() && symbol.length() <= 5
    if (!symbol.empty() && symbol.length() <= 5)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Side must be exactly "BUY" or "SELL"
inline bool is_valid_side(const std::string &side)
{
    // Done: side == "BUY" || side == "SELL"
    if (side == "BUY" || side == "SELL")
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Quantity must be strictly positive (int overload)
inline bool is_valid_qty(int qty)
{
    // Done: qty > 0
    if (qty > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Quantity must be strictly positive (double overload — for CSV-parsed data)
inline bool is_valid_qty(double qty)
{
    // Done: qty > 0.0
    if (qty > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Type must be "MARKET" or "LIMIT"
inline bool is_valid_type(const std::string &type)
{
    // Done
    if (type == "MARKET" || type == "LIMIT")
    {
        return true;
    }
    else
    {
        return false;
    }
}

// LIMIT orders require price > 0; MARKET orders are always price-valid
inline bool is_valid_price(const std::string &type, double price)
{
    // Done: if type == "LIMIT" return price > 0.0, else return true
    if (type == "LIMIT")
    {
        return (price > 0.0);
    }
    else
    {
        return true;
    }
}

// Master validator — runs all rules in order ─────────────────────────────────
inline ValidationResult validate(const TradeTicket &t)
{
    // Done: call each rule; return {false, reason} on first failure
    //       return {true, ""} if all rules pass
    if (!is_valid_symbol(t.symbol))
    {
        return {false, "not valid symbol"};
    }
    else if (!is_valid_side(t.side))
    {
        return {false, "not valid side"};
    }
    else if (!is_valid_qty(t.qty))
    {
        return {false, "not valid quantity"};
    }
    else if (!is_valid_type(t.type))
    {
        return {false, "not valid type"};
    }
    else if (!is_valid_price(t.type, t.price))
    {
        return {false, "not valid price"};
    }
    else
    {
        return {true, ""};
    }
}

#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <variant>
#include <atomic>
#include <cmath>
#include <numbers>
#include <stdexcept>

// ============================================================
// Shared types used across all platform subsystems
// ============================================================

enum class Side { BUY, SELL };
enum class OptionType { CALL, PUT };

struct Position {
    std::string symbol;
    int    qty{0};
    double avg_cost{0.0};
    double current_price{0.0};

    double market_value() const { return qty * current_price; }
    double unrealised_pnl() const { return qty * (current_price - avg_cost); }
};

struct Trade {
    std::string symbol;
    int    qty;
    double price;
    Side   side;
};

// Event bus types (backtester)
struct BarEvent   { std::string symbol; double open, high, low, close; long volume; };
struct SignalEvent { std::string symbol; int direction; };
struct OrderEvent  { std::string symbol; int quantity; double price; };
struct FillEvent   { std::string symbol; int quantity; double fill_price; double commission; };
using Event = std::variant<BarEvent, SignalEvent, OrderEvent, FillEvent>;

struct OptionParams {
    double S, K, r, sigma, T;
    OptionType type{OptionType::CALL};
};

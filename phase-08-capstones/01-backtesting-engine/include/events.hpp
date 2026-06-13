#pragma once
#include <string>
#include <variant>

struct BarEvent {
    std::string symbol;
    double open{0}, high{0}, low{0}, close{0};
    long   volume{0};
};

struct SignalEvent {
    std::string symbol;
    int  direction{0};   // +1 = BUY, -1 = SELL, 0 = FLAT
};

struct OrderEvent {
    std::string symbol;
    int    quantity{0};   // positive = buy, negative = sell
    double price{0};
};

struct FillEvent {
    std::string symbol;
    int    quantity{0};
    double fill_price{0};
    double commission{0};
};

using Event = std::variant<BarEvent, SignalEvent, OrderEvent, FillEvent>;

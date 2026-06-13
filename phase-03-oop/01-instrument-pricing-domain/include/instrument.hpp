#pragma once
#include <string>

// Abstract base class for all financial instruments.
class Instrument {
public:
    virtual double      price() const = 0;
    virtual std::string name()  const = 0;
    virtual ~Instrument() = default;
};

#pragma once
#include "instrument.hpp"
#include <cmath>
#include <string>

// Cumulative standard normal distribution via erfc approximation
inline double norm_cdf(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2.0));
}

class EuropeanOption : public Instrument {
public:
    enum class Type { CALL, PUT };

    EuropeanOption(std::string underlying, Type type,
                   double spot, double strike,
                   double rate, double vol, double T)
        : underlying_{std::move(underlying)}, type_{type},
          spot_{spot}, strike_{strike}, rate_{rate}, vol_{vol}, T_{T}
    {}

    // Black-Scholes formula
    // d1 = (ln(S/K) + (r + vol^2/2)*T) / (vol*sqrt(T))
    // d2 = d1 - vol*sqrt(T)
    // CALL = S*N(d1) - K*exp(-r*T)*N(d2)
    // PUT  = K*exp(-r*T)*N(-d2) - S*N(-d1)
    double price() const override;

    std::string name() const override {
        return underlying_ + (type_ == Type::CALL ? "_CALL" : "_PUT");
    }

private:
    std::string underlying_;
    Type        type_;
    double      spot_, strike_, rate_, vol_, T_;
};

inline double EuropeanOption::price() const {
    // TODO: implement Black-Scholes
    (void)spot_; (void)strike_; (void)rate_; (void)vol_; (void)T_;
    return 0.0;
}

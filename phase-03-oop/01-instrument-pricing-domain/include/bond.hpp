#pragma once
#include "instrument.hpp"
#include <cmath>
#include <utility>

class Bond : public Instrument {
public:
    Bond(std::string symbol, double face, double coupon_rate, double ytm, int years)
        : symbol_{std::move(symbol)}, face_{face},
          coupon_rate_{coupon_rate}, ytm_{ytm}, years_{years}
    {}

    // Present value of coupons + face:
    // PV = Σ_{t=1}^{years} coupon*face/(1+ytm)^t  +  face/(1+ytm)^years
    double price() const override;

    std::string name() const override { return symbol_; }

    double face()        const { return face_; }
    double coupon_rate() const { return coupon_rate_; }
    double ytm()         const { return ytm_; }
    int    years()       const { return years_; }

private:
    std::string symbol_;
    double face_{1000.0};
    double coupon_rate_{0.0};
    double ytm_{0.0};
    int    years_{1};
};

inline double Bond::price() const {
    // TODO: implement bond present-value formula
    (void)face_; (void)coupon_rate_; (void)ytm_; (void)years_;
    return 0.0;
}

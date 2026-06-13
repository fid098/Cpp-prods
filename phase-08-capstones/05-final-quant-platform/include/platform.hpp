#pragma once
#include "platform_types.hpp"
#include <memory>
#include <thread>
#include <random>
#include <future>
#include <deque>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iostream>

// ============================================================
// BSM Pricer
// ============================================================
namespace pricing {
    inline double norm_cdf(double x) { return 0.5 * std::erfc(-x / std::numbers::sqrt2); }
    inline double norm_pdf(double x) { return std::exp(-0.5*x*x) / std::sqrt(2.0*std::numbers::pi); }

    struct Greeks { double price, delta, gamma, vega, theta; };

    inline Greeks bsm(double S, double K, double r, double sigma, double T, OptionType type) {
        if (S<=0||K<=0||sigma<=0||T<=0) throw std::invalid_argument("invalid BSM params");
        double d1 = (std::log(S/K)+(r+0.5*sigma*sigma)*T)/(sigma*std::sqrt(T));
        double d2 = d1 - sigma*std::sqrt(T);
        double df = std::exp(-r*T), sqT = std::sqrt(T), pdf1 = norm_pdf(d1);
        Greeks g;
        if (type == OptionType::CALL) {
            g.price = S*norm_cdf(d1) - K*df*norm_cdf(d2);
            g.delta = norm_cdf(d1);
            g.theta = -(S*pdf1*sigma/(2*sqT)) - r*K*df*norm_cdf(d2);
        } else {
            g.price = K*df*norm_cdf(-d2) - S*norm_cdf(-d1);
            g.delta = norm_cdf(d1) - 1.0;
            g.theta = -(S*pdf1*sigma/(2*sqT)) + r*K*df*norm_cdf(-d2);
        }
        g.gamma = pdf1 / (S*sigma*sqT);
        g.vega  = S*pdf1*sqT;
        return g;
    }
}

// ============================================================
// Order Book
// ============================================================
class OrderBook {
public:
    struct Level { int total_qty{0}; };

    void add(Side side, int qty, double price) {
        if (side == Side::BUY) bids_[price] += qty;
        else                   asks_[price] += qty;
    }

    double best_bid() const { return bids_.empty() ? 0.0 : bids_.rbegin()->first; }
    double best_ask() const { return asks_.empty() ? 0.0 : asks_.begin()->first; }
    double spread()   const { return best_ask() - best_bid(); }

    std::string display(const std::string& symbol) const {
        std::ostringstream os;
        os << std::fixed << std::setprecision(2);
        os << "=== " << symbol << " Order Book ===\n";
        int rows = 0;
        for (auto it = asks_.rbegin(); it != asks_.rend() && rows < 5; ++it, ++rows)
            os << "  ASK " << std::setw(10) << it->first << "  qty=" << it->second << "\n";
        os << "  --- spread=" << spread() << " ---\n";
        rows = 0;
        for (auto it = bids_.rbegin(); it != bids_.rend() && rows < 5; ++it, ++rows)
            os << "  BID " << std::setw(10) << it->first << "  qty=" << it->second << "\n";
        return os.str();
    }

private:
    std::map<double, int> bids_, asks_;
};

// ============================================================
// Portfolio Manager
// ============================================================
class PortfolioManager {
public:
    explicit PortfolioManager(double cash) : cash_{cash}, initial_cash_{cash} {}

    void fill(const FillEvent& f) {
        auto& pos = positions_[f.symbol];
        pos.symbol = f.symbol;
        if (f.quantity > 0) {
            double total_cost = pos.qty * pos.avg_cost + f.quantity * f.fill_price;
            pos.qty += f.quantity;
            pos.avg_cost = (pos.qty > 0) ? total_cost / pos.qty : 0.0;
        } else {
            pos.qty += f.quantity;
        }
        cash_ -= f.quantity * f.fill_price + f.commission;
    }

    void mark_to_market(const std::string& sym, double price) {
        if (positions_.count(sym)) positions_[sym].current_price = price;
    }

    double total_pnl() const {
        double unrealised = 0.0;
        for (auto& [s, p] : positions_) unrealised += p.unrealised_pnl();
        return cash_ - initial_cash_ + unrealised;
    }

    double cash() const { return cash_; }
    const std::unordered_map<std::string, Position>& positions() const { return positions_; }

private:
    double cash_, initial_cash_;
    std::unordered_map<std::string, Position> positions_;
};

// ============================================================
// Risk Engine
// ============================================================
class RiskEngine {
public:
    void add_returns(const std::vector<double>& r) { history_ = r; }

    double var95() const { return historical_var(history_, 0.95); }
    double es95()  const { return expected_shortfall(history_, 0.95); }
    double sharpe() const {
        if (history_.empty()) return 0.0;
        double m = mean(history_), s = std_dev(history_);
        return (s > 0) ? m / s * std::sqrt(252.0) : 0.0;
    }

private:
    std::vector<double> history_;

    static double mean(const std::vector<double>& v) {
        return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
    }
    static double std_dev(const std::vector<double>& v) {
        double m = mean(v), var = 0.0;
        for (double x : v) var += (x-m)*(x-m);
        return std::sqrt(var / v.size());
    }
    static double historical_var(std::vector<double> v, double conf) {
        if (v.empty()) return 0.0;
        std::sort(v.begin(), v.end());
        return -v[static_cast<std::size_t>((1-conf)*v.size())];
    }
    static double expected_shortfall(std::vector<double> v, double conf) {
        if (v.empty()) return 0.0;
        std::sort(v.begin(), v.end());
        int cut = static_cast<int>((1-conf)*v.size());
        if (cut <= 0) return -v.front();
        double s = 0.0;
        for (int i = 0; i < cut; ++i) s += v[i];
        return -s/cut;
    }
};

// ============================================================
// Simple Backtester
// ============================================================
class Backtester {
public:
    struct Result { double total_return, sharpe, max_drawdown; int trades; };

    Result run(const std::vector<BarEvent>& bars, int fast=5, int slow=20, double cash=1e6) {
        std::deque<Event> q;
        std::vector<double> prices;
        bool   long_pos = false;
        double port_cash = cash;
        int    holding   = 0;
        std::vector<double> equity = {cash};

        for (auto& bar : bars) {
            prices.push_back(bar.close);
            int n = static_cast<int>(prices.size());
            if (n >= slow) {
                double fa = sma(prices, n-fast, fast);
                double sl = sma(prices, n-slow, slow);
                if (fa > sl && !long_pos) {
                    int qty = static_cast<int>(port_cash / bar.close);
                    port_cash -= qty * bar.close;
                    holding    = qty;
                    long_pos   = true;
                } else if (fa < sl && long_pos) {
                    port_cash += holding * bar.close;
                    holding    = 0;
                    long_pos   = false;
                }
            }
            equity.push_back(port_cash + holding * bar.close);
        }

        double final_eq = equity.back();
        std::vector<double> rets;
        for (std::size_t i = 1; i < equity.size(); ++i)
            rets.push_back((equity[i]-equity[i-1])/equity[i-1]);

        Result r;
        r.total_return  = (final_eq - cash) / cash;
        r.trades = 0; // simplified
        double m = rets.empty() ? 0.0 : std::accumulate(rets.begin(), rets.end(), 0.0)/rets.size();
        double s = 0.0;
        for (double x : rets) s += (x-m)*(x-m);
        s = rets.size() > 1 ? std::sqrt(s/rets.size()) : 0.0;
        r.sharpe = s > 0 ? m/s*std::sqrt(252.0) : 0.0;
        // max drawdown
        double peak = equity[0], dd = 0.0;
        for (double e : equity) { if (e>peak) peak=e; if (peak>0) dd=std::max(dd,(peak-e)/peak); }
        r.max_drawdown = dd;
        return r;
    }

private:
    static double sma(const std::vector<double>& v, int start, int len) {
        double s = 0.0;
        for (int i = start; i < start+len; ++i) s += v[i];
        return s/len;
    }
};

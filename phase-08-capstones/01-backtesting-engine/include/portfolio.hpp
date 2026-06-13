#pragma once
#include "events.hpp"
#include <unordered_map>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <stdexcept>

struct PerformanceReport {
    double total_return{0};
    double annualised_sharpe{0};
    double max_drawdown{0};
    int    total_trades{0};
    double final_equity{0};
};

class Portfolio {
public:
    explicit Portfolio(double initial_cash)
        : cash_{initial_cash}, initial_equity_{initial_cash} {
        equity_curve_.push_back(initial_cash);
    }

    void on_fill(const FillEvent& fill) {
        positions_[fill.symbol] += fill.quantity;
        cash_ -= fill.quantity * fill.fill_price + fill.commission;
        ++total_trades_;
    }

    void on_bar(const BarEvent& bar) {
        last_prices_[bar.symbol] = bar.close;
        double equity = cash_;
        for (auto& [sym, qty] : positions_) {
            auto it = last_prices_.find(sym);
            if (it != last_prices_.end()) equity += qty * it->second;
        }
        equity_curve_.push_back(equity);
    }

    PerformanceReport report() const {
        if (equity_curve_.size() < 2) throw std::runtime_error("no data");

        PerformanceReport r;
        r.final_equity  = equity_curve_.back();
        r.total_return  = (r.final_equity - initial_equity_) / initial_equity_;
        r.total_trades  = total_trades_;
        r.annualised_sharpe = compute_sharpe();
        r.max_drawdown  = compute_max_drawdown();
        return r;
    }

    const std::vector<double>& equity_curve() const { return equity_curve_; }
    double cash() const { return cash_; }
    int position(const std::string& sym) const {
        auto it = positions_.find(sym);
        return it != positions_.end() ? it->second : 0;
    }

private:
    double initial_equity_;
    double cash_;
    int    total_trades_{0};
    std::unordered_map<std::string, int>    positions_;
    std::unordered_map<std::string, double> last_prices_;
    std::vector<double> equity_curve_;

    double compute_sharpe() const {
        std::vector<double> rets;
        for (std::size_t i = 1; i < equity_curve_.size(); ++i)
            rets.push_back((equity_curve_[i] - equity_curve_[i-1]) / equity_curve_[i-1]);
        if (rets.empty()) return 0.0;
        double mean = std::accumulate(rets.begin(), rets.end(), 0.0) / rets.size();
        double var  = 0.0;
        for (double r : rets) var += (r - mean) * (r - mean);
        var /= rets.size();
        double std_dev = std::sqrt(var);
        if (std_dev == 0.0) return 0.0;
        return mean / std_dev * std::sqrt(252.0);
    }

    double compute_max_drawdown() const {
        double peak = equity_curve_[0], max_dd = 0.0;
        for (double eq : equity_curve_) {
            if (eq > peak) peak = eq;
            if (peak > 0.0) max_dd = std::max(max_dd, (peak - eq) / peak);
        }
        return max_dd;
    }
};

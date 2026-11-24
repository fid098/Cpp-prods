#include <iomanip>
#include <iostream>
#include <vector>
#include "account.hpp"

// Prints one account's order report. Takes `const Account&` — it only needs
// the shared interface (kind(), balance()), so it works for every subclass.
void print_report(const Account& account, double opening,
                  const std::vector<Order>& orders) {
    std::cout << account.kind() << " account  opening balance=$" << opening << "\n";
    for (const auto& o : orders) {
        const bool filled = o.status == Order::Status::FILLED;
        std::cout << "  " << std::left << std::setw(5) << (o.is_buy ? "BUY" : "SELL")
                  << std::setw(5) << o.symbol
                  << std::right << std::setw(4) << o.qty << " @ "
                  << std::setw(7) << o.price
                  << "  fee=$" << std::setw(5) << o.fee_paid
                  << "  " << std::left << std::setw(8) << (filled ? "FILLED" : "REJECTED")
                  << std::right << "  balance=$" << o.balance_after << "\n";
    }
    std::cout << "\n";
}

int main() {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "=== Brokerage Simulator ===\n\n";

    // ── 1. Same function, two account types ─────────────────────────────────
    CashAccount cash{10000.0};
    std::vector<Order> cash_orders = {
        {"AAPL", 50, 150.0, true},   // 7,500 + $1 fee → OK
        {"TSLA", 10, 700.0, true},   // 7,000 > remaining cash → REJECTED
        {"AAPL", 50, 160.0, false},  // sell at a profit
    };
    process_orders(cash, cash_orders);
    print_report(cash, 10000.0, cash_orders);

    MarginAccount margin{10000.0, 2.0};
    std::vector<Order> margin_orders = {
        {"TSLA", 20, 700.0, true},   // 14,000 — more than the deposit, OK on margin
        {"NVDA", 10, 800.0, true},   // would exceed 2x leverage → REJECTED
        {"TSLA", 20, 720.0, false},  // close the position, repay the loan
    };
    process_orders(margin, margin_orders);
    print_report(margin, 10000.0, margin_orders);

    // ── 2. Heterogeneous collection through base pointers ───────────────────
    // The vector holds Account* — it doesn't know or care about the concrete
    // types. (Non-owning pointers to stack objects: nothing to delete.)
    std::vector<Account*> accounts = {&cash, &margin};

    std::cout << "--- Fee on a $100,000 trade (virtual dispatch) ---\n";
    for (const Account* acc : accounts) {
        std::cout << "  " << std::left << std::setw(7) << acc->kind() << std::right
                  << "fee=$" << acc->fee(100000.0) << "\n";
    }
    std::cout << "\n";

    // ── 3. dynamic_cast: recover the concrete type at run time ──────────────
    // max_leverage()/buying_power() exist only on MarginAccount, so they can't
    // be called through Account*. dynamic_cast checks the real type and
    // returns nullptr if the object is not a MarginAccount.
    std::cout << "--- Margin details (dynamic_cast) ---\n";
    for (Account* acc : accounts) {
        if (auto* m = dynamic_cast<MarginAccount*>(acc)) {
            std::cout << "  " << acc->kind() << ": leverage=" << m->max_leverage()
                      << "x  credit_limit=$" << m->credit_limit()
                      << "  buying_power=$" << m->buying_power() << "\n";
        } else {
            std::cout << "  " << acc->kind() << ": not a margin account (cast returned nullptr)\n";
        }
    }

    return 0;
}

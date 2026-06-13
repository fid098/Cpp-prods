#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>
#include <chrono>
#include <random>
#include <memory>
#include <string>
#include <vector>
#include "platform.hpp"

// ============================================================
// Live Market Data (simulated with atomics)
// ============================================================
struct InstrumentTick {
    std::atomic<double> bid{0}, ask{0}, last{0};
    std::atomic<bool>   valid{false};
};

struct MarketData {
    static constexpr int N = 6;
    const char* symbols[N] = {"AAPL","MSFT","TSLA","AMZN","NVDA","GOOGL"};
    InstrumentTick ticks[N];
    std::atomic<long long> updates{0};

    MarketData() {
        double prices[N] = {175,420,250,190,875,175};
        for (int i=0;i<N;++i) {
            double sp = prices[i]*0.0002;
            ticks[i].bid.store(prices[i]-sp, std::memory_order_relaxed);
            ticks[i].ask.store(prices[i]+sp, std::memory_order_relaxed);
            ticks[i].last.store(prices[i],   std::memory_order_relaxed);
            ticks[i].valid.store(true,        std::memory_order_release);
        }
    }
};

// ============================================================
// CLI helpers
// ============================================================
static void print_header() {
    std::cout << "\033[1;36m"
              << " ╔═══════════════════════════════════════════╗\n"
              << " ║        FINAL QUANT PLATFORM v1.0          ║\n"
              << " ╚═══════════════════════════════════════════╝\n"
              << "\033[0m";
}

static void show_feed(const MarketData& md) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n  Symbol      Bid         Ask         Last\n";
    std::cout << "  ─────────────────────────────────────────\n";
    for (int i = 0; i < MarketData::N; ++i) {
        auto& t = md.ticks[i];
        if (!t.valid.load(std::memory_order_acquire)) continue;
        std::cout << "  " << std::left << std::setw(8) << md.symbols[i]
                  << std::right << std::setw(10) << t.bid.load(std::memory_order_acquire)
                  << std::setw(12) << t.ask.load(std::memory_order_acquire)
                  << std::setw(12) << t.last.load(std::memory_order_acquire) << "\n";
    }
    std::cout << "  Total updates: " << md.updates.load(std::memory_order_relaxed) << "\n";
}

static void show_help() {
    std::cout << "\n  Commands:\n"
              << "    feed                   — show live market data\n"
              << "    book                   — show AAPL order book\n"
              << "    price S K r sig T      — BSM call price\n"
              << "    backtest               — run SMA crossover backtest\n"
              << "    risk                   — compute portfolio VaR/ES/Sharpe\n"
              << "    portfolio              — show positions and PnL\n"
              << "    help                   — show this menu\n"
              << "    quit                   — exit\n\n";
}

int main() {
    print_header();
    show_help();

    // ---- Subsystems --------------------------------------------------------
    auto mkt  = std::make_unique<MarketData>();
    auto book = std::make_unique<OrderBook>();
    auto port = std::make_unique<PortfolioManager>(1'000'000.0);
    auto risk = std::make_unique<RiskEngine>();
    auto bt   = std::make_unique<Backtester>();

    // Seed order book
    book->add(Side::BUY,  200, 174.85); book->add(Side::BUY,  100, 174.70);
    book->add(Side::SELL, 150, 175.10); book->add(Side::SELL, 300, 175.25);

    // ---- Background publisher ----------------------------------------------
    std::jthread publisher([&](std::stop_token st) {
        thread_local std::mt19937 rng{std::random_device{}()};
        std::normal_distribution<double> norm{0.0, 1.0};
        double prices[MarketData::N] = {175,420,250,190,875,175};
        double vols[MarketData::N]   = {0.25,0.22,0.50,0.28,0.45,0.23};
        double dt = 1.0/(252.0*23400.0);
        while (!st.stop_requested()) {
            for (int i=0;i<MarketData::N;++i) {
                double Z = norm(rng);
                prices[i] *= std::exp(-0.5*vols[i]*vols[i]*dt + vols[i]*std::sqrt(dt)*Z);
                double sp = prices[i]*0.0002;
                mkt->ticks[i].bid.store(prices[i]-sp,  std::memory_order_relaxed);
                mkt->ticks[i].ask.store(prices[i]+sp,  std::memory_order_relaxed);
                mkt->ticks[i].last.store(prices[i],    std::memory_order_release);
            }
            mkt->updates.fetch_add(1, std::memory_order_relaxed);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });

    // ---- REPL --------------------------------------------------------------
    std::string line;
    std::cout << "qp> ";
    while (std::getline(std::cin, line)) {
        std::istringstream ss{line};
        std::string cmd; ss >> cmd;

        if (cmd == "feed") {
            show_feed(*mkt);
        } else if (cmd == "book") {
            std::cout << book->display("AAPL");
        } else if (cmd == "price") {
            double S, K, r, sig, T;
            if (ss >> S >> K >> r >> sig >> T) {
                try {
                    auto g = pricing::bsm(S, K, r, sig, T, OptionType::CALL);
                    std::cout << std::fixed << std::setprecision(4)
                              << "  Call=" << g.price << "  Delta=" << g.delta
                              << "  Gamma=" << g.gamma << "  Vega=" << g.vega << "\n";
                } catch (const std::exception& e) {
                    std::cout << "  Error: " << e.what() << "\n";
                }
            } else {
                std::cout << "  Usage: price S K r sigma T\n";
            }
        } else if (cmd == "backtest") {
            // Synthetic data
            std::vector<BarEvent> bars;
            double p = 100.0;
            std::mt19937 rng{99};
            std::normal_distribution<double> dist{0.0003, 0.012};
            for (int i = 0; i < 252; ++i) {
                p *= (1.0 + dist(rng));
                bars.push_back({"SYM", p*0.99, p*1.01, p*0.98, p, 100000});
            }
            auto r = bt->run(bars);
            std::cout << std::fixed << std::setprecision(4)
                      << "  Return:      " << r.total_return*100.0 << "%\n"
                      << "  Sharpe:      " << r.sharpe << "\n"
                      << "  MaxDrawdown: " << r.max_drawdown*100.0 << "%\n";
        } else if (cmd == "risk") {
            // Generate synthetic returns for demo
            std::mt19937 rng{42};
            std::normal_distribution<double> d{0.0005, 0.012};
            std::vector<double> rets(252);
            for (auto& x : rets) x = d(rng);
            risk->add_returns(rets);
            std::cout << std::fixed << std::setprecision(4)
                      << "  VaR (95%):  " << risk->var95()*100.0 << "%\n"
                      << "  ES  (95%):  " << risk->es95() *100.0 << "%\n"
                      << "  Sharpe:     " << risk->sharpe() << "\n";
        } else if (cmd == "portfolio") {
            std::cout << "  Cash: $" << std::fixed << std::setprecision(2)
                      << port->cash() << "\n";
            for (auto& [sym, pos] : port->positions()) {
                std::cout << "  " << sym << ": qty=" << pos.qty
                          << " avg=" << pos.avg_cost
                          << " pnl=$" << pos.unrealised_pnl() << "\n";
            }
            std::cout << "  Total PnL: $" << port->total_pnl() << "\n";
        } else if (cmd == "help") {
            show_help();
        } else if (cmd == "quit" || cmd == "exit" || cmd == "q") {
            break;
        } else if (!cmd.empty()) {
            std::cout << "  Unknown command. Type 'help'.\n";
        }

        std::cout << "qp> ";
    }

    std::cout << "\nGoodbye.\n";
    return 0;
}

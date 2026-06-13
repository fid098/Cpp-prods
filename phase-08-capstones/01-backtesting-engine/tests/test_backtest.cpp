#include <gtest/gtest.h>
#include "events.hpp"
#include "bar_feed.hpp"
#include "strategy.hpp"
#include "broker.hpp"
#include "portfolio.hpp"
#include <deque>
#include <variant>

// Helper: run the event loop for one bar
static void process_bar(const BarEvent& bar, Strategy& strat, SimBroker& broker,
                         Portfolio& port, std::deque<Event>& q, int qty = 100) {
    q.push_back(bar);
    while (!q.empty()) {
        Event e = q.front(); q.pop_front();
        std::visit([&](auto&& ev) {
            using T = std::decay_t<decltype(ev)>;
            if constexpr (std::is_same_v<T, BarEvent>) {
                strat.on_bar(ev, q);
                port.on_bar(ev);
            } else if constexpr (std::is_same_v<T, SignalEvent>) {
                if (ev.direction != 0)
                    q.push_back(OrderEvent{ev.symbol, ev.direction * qty, ev.direction > 0 ? 100.0 : 110.0});
            } else if constexpr (std::is_same_v<T, OrderEvent>) {
                broker.on_order(ev, q);
            } else if constexpr (std::is_same_v<T, FillEvent>) {
                port.on_fill(ev);
            }
        }, e);
    }
}

TEST(Portfolio, InitialEquityMatchesCash) {
    Portfolio p{50000.0};
    EXPECT_DOUBLE_EQ(p.cash(), 50000.0);
}

TEST(Portfolio, FillDeductsCash) {
    Portfolio p{100000.0};
    p.on_fill(FillEvent{"AAPL", 100, 175.0, 17.5});
    EXPECT_NEAR(p.cash(), 100000.0 - 100*175.0 - 17.5, 1e-6);
}

TEST(Portfolio, PositionTracked) {
    Portfolio p{100000.0};
    p.on_fill(FillEvent{"AAPL", 100, 175.0, 0});
    EXPECT_EQ(p.position("AAPL"), 100);
    p.on_fill(FillEvent{"AAPL", -50, 180.0, 0});
    EXPECT_EQ(p.position("AAPL"), 50);
}

TEST(Portfolio, ReportRequiresEnoughData) {
    Portfolio p{100000.0};
    EXPECT_THROW(p.report(), std::runtime_error);
}

TEST(SimBroker, FillGeneratedForValidOrder) {
    SimBroker broker{0.001};
    std::deque<Event> q;
    broker.on_order(OrderEvent{"AAPL", 100, 175.0}, q);
    ASSERT_EQ(q.size(), 1u);
    auto& fill = std::get<FillEvent>(q.front());
    EXPECT_EQ(fill.quantity, 100);
    EXPECT_DOUBLE_EQ(fill.fill_price, 175.0);
    EXPECT_NEAR(fill.commission, 100*175.0*0.001, 1e-9);
}

TEST(SimBroker, ZeroQtyIgnored) {
    SimBroker broker;
    std::deque<Event> q;
    broker.on_order(OrderEvent{"AAPL", 0, 175.0}, q);
    EXPECT_TRUE(q.empty());
}

TEST(SMAStrategy, NoSignalBeforeEnoughData) {
    SMACrossover strat{5, 20};
    std::deque<Event> q;
    for (int i = 0; i < 19; ++i)
        strat.on_bar(BarEvent{"SYM", 100, 101, 99, 100.0+i*0.1, 1000}, q);
    EXPECT_TRUE(q.empty());  // need 20 bars before first signal possible
}

TEST(BarFeed, InMemoryFeedWorks) {
    std::vector<BarEvent> bars = {
        {"X", 10, 11, 9, 10, 1000},
        {"X", 11, 12, 10, 11, 1000}
    };
    BarFeed feed{bars};
    EXPECT_TRUE(feed.has_next());
    auto b = feed.next();
    EXPECT_EQ(b.symbol, "X");
    EXPECT_DOUBLE_EQ(b.close, 10.0);
    EXPECT_TRUE(feed.has_next());
    feed.next();
    EXPECT_FALSE(feed.has_next());
}

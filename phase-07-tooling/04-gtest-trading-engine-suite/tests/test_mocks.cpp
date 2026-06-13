#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "trading_engine.hpp"
#include "risk_checker.hpp"

// ---- GMock: mock the risk checker interface --------------------------------

class MockRiskChecker : public IRiskChecker {
public:
    MOCK_METHOD(bool, check_order, (int qty, double price), (const, override));
};

TEST(EngineWithMock, AcceptsOrderWhenRiskPasses) {
    MockRiskChecker mock;
    EXPECT_CALL(mock, check_order(100, 175.0))
        .WillOnce(::testing::Return(true));

    TradingEngine engine{&mock};
    EXPECT_TRUE(engine.submit_order(100, 175.0));
    EXPECT_EQ(engine.book().order_count(), 1);
}

TEST(EngineWithMock, RejectsOrderWhenRiskFails) {
    MockRiskChecker mock;
    EXPECT_CALL(mock, check_order(::testing::_, ::testing::_))
        .WillRepeatedly(::testing::Return(false));

    TradingEngine engine{&mock};
    EXPECT_FALSE(engine.submit_order(100, 175.0));
    EXPECT_EQ(engine.book().order_count(), 0);
}

TEST(EngineWithMock, RiskCheckerCalledExactlyOnce) {
    MockRiskChecker mock;
    EXPECT_CALL(mock, check_order(50, 200.0)).Times(1).WillOnce(::testing::Return(true));

    TradingEngine engine{&mock};
    engine.submit_order(50, 200.0);
}

TEST(EngineWithMock, MultipleOrdersEachTriggerRiskCheck) {
    MockRiskChecker mock;
    EXPECT_CALL(mock, check_order(::testing::_, ::testing::_))
        .Times(3)
        .WillRepeatedly(::testing::Return(true));

    TradingEngine engine{&mock};
    engine.submit_order(10, 100.0);
    engine.submit_order(20, 200.0);
    engine.submit_order(30, 300.0);
    EXPECT_EQ(engine.book().order_count(), 3);
}

// ---- Production risk checker -------------------------------------------

TEST(SimpleRiskChecker, PassesWithinLimit) {
    SimpleRiskChecker r{100'000.0};
    EXPECT_TRUE(r.check_order(100, 500.0));   // 50,000 < 100,000
}

TEST(SimpleRiskChecker, RejectsAboveLimit) {
    SimpleRiskChecker r{100'000.0};
    EXPECT_FALSE(r.check_order(1000, 200.0)); // 200,000 > 100,000
}

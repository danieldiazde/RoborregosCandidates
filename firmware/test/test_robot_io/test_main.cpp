#include <unity.h>
#include "FakeRobotIO.hpp"

using namespace maze;

void setUp() {}
void tearDown() {}

void test_advance_moves_and_costs_time() {
    Maze m;
    m.setWall(0, 0, Direction::North, WallState::Open);
    FakeRobotIO io(m, 0, 0, Direction::North);

    TEST_ASSERT_EQUAL(static_cast<int>(ActionResult::Ok), static_cast<int>(io.advance()));
    TEST_ASSERT_EQUAL(0, io.x());
    TEST_ASSERT_EQUAL(1, io.y());
    TEST_ASSERT_EQUAL_UINT16(1, io.moveCount());
    TEST_ASSERT_EQUAL_UINT32(3000, io.elapsedMs());
    TEST_ASSERT_EQUAL_UINT32(357000, io.millisRemaining());
}

void test_blocked_advance_does_not_move() {
    Maze m;   // interior walls Unknown, so nothing is Open
    FakeRobotIO io(m, 0, 0, Direction::North);

    TEST_ASSERT_EQUAL(static_cast<int>(ActionResult::Blocked), static_cast<int>(io.advance()));
    TEST_ASSERT_EQUAL(0, io.y());
    TEST_ASSERT_EQUAL_UINT16(1, io.blockedCount());
    TEST_ASSERT_EQUAL_UINT32(0, io.elapsedMs());
}

void test_turn_counts_and_costs() {
    Maze m;
    FakeRobotIO io(m, 0, 0, Direction::North);

    io.turn(RelativeTurn::Right);
    TEST_ASSERT_EQUAL(static_cast<int>(Direction::East), static_cast<int>(io.heading()));
    io.turn(RelativeTurn::Around);
    TEST_ASSERT_EQUAL(static_cast<int>(Direction::West), static_cast<int>(io.heading()));

    TEST_ASSERT_EQUAL_UINT16(2, io.turnCount());
    TEST_ASSERT_EQUAL_UINT32(1500 + 2500, io.elapsedMs());
}

void test_clock_saturates_at_zero() {
    Maze m;
    FakeRobotIO io(m, 0, 0, Direction::North);
    io.setMillisRemaining(1000);
    io.turn(RelativeTurn::Left);
    TEST_ASSERT_EQUAL_UINT32(0, io.millisRemaining());
}

int main(int, char**) {
    UNITY_BEGIN();
    RUN_TEST(test_advance_moves_and_costs_time);
    RUN_TEST(test_blocked_advance_does_not_move);
    RUN_TEST(test_turn_counts_and_costs);
    RUN_TEST(test_clock_saturates_at_zero);
    return UNITY_END();
}
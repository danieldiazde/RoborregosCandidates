// firmware/test/test_path_planner/test_main.cpp
#include <unity.h>
#include "PathPlanner.hpp"

using namespace maze;

void setUp() {}
void tearDown() {}

#define ASSERT_STEP(expected, actual)                                   \
    do {                                                                \
        auto _s = (actual);                                             \
        TEST_ASSERT_TRUE(_s.has_value());                               \
        TEST_ASSERT_EQUAL(static_cast<int>(expected), static_cast<int>(*_s)); \
    } while (0)

void test_straight_corridor() {
    Maze m;
    m.setWall(0, 0, Direction::North, WallState::Open);
    m.setWall(0, 1, Direction::North, WallState::Open);
    ASSERT_STEP(Direction::North, firstStepToward(m, 0, 0, 0, 2));
}

void test_goes_around_a_wall() {
    // (0,0) → (1,0) → (1,1) → (0,1); the direct wall (0,0)-(0,1) is closed
    Maze m;
    m.setWall(0, 0, Direction::North, WallState::Blocked);
    m.setWall(0, 0, Direction::East,  WallState::Open);
    m.setWall(1, 0, Direction::North, WallState::Open);
    m.setWall(1, 1, Direction::West,  WallState::Open);
    ASSERT_STEP(Direction::East, firstStepToward(m, 0, 0, 0, 1));
}

void test_picks_the_shorter_route() {
    // Two routes to (2,0): direct east (2 steps) or north-and-around (4 steps)
    Maze m;
    m.setWall(0, 0, Direction::East,  WallState::Open);
    m.setWall(1, 0, Direction::East,  WallState::Open);
    m.setWall(0, 0, Direction::North, WallState::Open);
    m.setWall(0, 1, Direction::East,  WallState::Open);
    m.setWall(1, 1, Direction::East,  WallState::Open);
    m.setWall(2, 1, Direction::South, WallState::Open);
    ASSERT_STEP(Direction::East, firstStepToward(m, 0, 0, 2, 0));
}

void test_unknown_walls_are_not_paths() {
    Maze m;   // interior all Unknown
    TEST_ASSERT_FALSE(firstStepToward(m, 0, 0, 1, 0).has_value());
}

void test_unreachable_target() {
    Maze m;
    m.setWall(0, 0, Direction::North, WallState::Open);
    TEST_ASSERT_FALSE(firstStepToward(m, 0, 0, 4, 4).has_value());
}

void test_already_there() {
    Maze m;
    TEST_ASSERT_FALSE(firstStepToward(m, 2, 2, 2, 2).has_value());
}

void test_never_routes_through_red() {
    // Only route to (0,2) passes through (0,1), which is red
    Maze m;
    m.setWall(0, 0, Direction::North, WallState::Open);
    m.setWall(0, 1, Direction::North, WallState::Open);
    m.cell(0, 1).setColor(TileColor::Red);
    TEST_ASSERT_FALSE(firstStepToward(m, 0, 0, 0, 2).has_value());
}

void test_red_is_allowed_as_the_target() {
    Maze m;
    m.setWall(0, 0, Direction::North, WallState::Open);
    m.cell(0, 1).setColor(TileColor::Red);
    ASSERT_STEP(Direction::North, firstStepToward(m, 0, 0, 0, 1));
}

int main(int, char**) {
    UNITY_BEGIN();
    RUN_TEST(test_straight_corridor);
    RUN_TEST(test_goes_around_a_wall);
    RUN_TEST(test_picks_the_shorter_route);
    RUN_TEST(test_unknown_walls_are_not_paths);
    RUN_TEST(test_unreachable_target);
    RUN_TEST(test_already_there);
    RUN_TEST(test_never_routes_through_red);
    RUN_TEST(test_red_is_allowed_as_the_target);
    return UNITY_END();
}
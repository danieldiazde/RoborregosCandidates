// firmware/test/test_strategies/test_main.cpp
#include <unity.h>
#include "MazeGenerator.hpp"
#include "FakeRobotIO.hpp"
#include "MazeSolver.hpp"
#include "WallFollower.hpp"

using namespace maze;

void setUp() {}
void tearDown() {}

static const MotionCosts kFree{1, 1, 1, 1};   // correctness tests, not timing

static RunResult runWallFollower(std::uint32_t seed, std::uint8_t loops,
                                 std::uint16_t& blocked) {
    GeneratorConfig c;
    c.seed = seed;
    c.extraOpenings = loops;
    GeneratedMaze g = MazeGenerator(c).generate();

    FakeRobotIO io(g.maze, g.startX, g.startY, g.startHeading, kFree);
    WallFollower strategy;
    MazeSolver solver(io, strategy);

    RunResult r = solver.run();
    blocked = io.blockedCount();
    return r;
}

void test_wall_follower_covers_perfect_mazes() {
    for (std::uint32_t seed = 0; seed < 50; ++seed) {
        std::uint16_t blocked;
        RunResult r = runWallFollower(seed, 0, blocked);
        TEST_ASSERT_EQUAL(25, r.cellsVisited);
        TEST_ASSERT_FALSE(r.strategyErr);
        TEST_ASSERT_EQUAL(0, blocked);
    }
}

void test_wall_follower_terminates_on_looped_mazes() {
    for (std::uint32_t seed = 0; seed < 50; ++seed) {
        std::uint16_t blocked;
        RunResult r = runWallFollower(seed, 8, blocked);
        TEST_ASSERT_FALSE(r.strategyErr);
        TEST_ASSERT_EQUAL(0, blocked);
        TEST_ASSERT_LESS_THAN(MazeSolver::kMaxSteps, r.steps);
    }
}

int main(int, char**) {
    UNITY_BEGIN();
    RUN_TEST(test_wall_follower_covers_perfect_mazes);
    RUN_TEST(test_wall_follower_terminates_on_looped_mazes);
    return UNITY_END();
}
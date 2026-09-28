// firmware/test/test_strategies/test_main.cpp
#include <unity.h>
#include "MazeGenerator.hpp"
#include "FakeRobotIO.hpp"
#include "MazeSolver.hpp"
#include "WallFollower.hpp"
#include "DfsExplorer.hpp"
#include "DfsStraightFirst.hpp"

using namespace maze;

void setUp() {}
void tearDown() {}

static const MotionCosts kFree{1, 1, 1, 1};   // correctness tests, not timing

template <typename Strategy>
static RunResult runStrategy(std::uint32_t seed, std::uint8_t loops,
                             std::uint16_t& moves, std::uint16_t& blocked) {
    GeneratorConfig c;
    c.seed = seed;
    c.extraOpenings = loops;
    GeneratedMaze g = MazeGenerator(c).generate();

    FakeRobotIO io(g.maze, g.startX, g.startY, g.startHeading, kFree);
    Strategy strategy;
    MazeSolver solver(io, strategy);

    RunResult r = solver.run();
    moves   = io.moveCount();
    blocked = io.blockedCount();
    return r;
}

// ---------------- WallFollower ----------------

void test_wall_follower_covers_perfect_mazes() {
    for (std::uint32_t seed = 0; seed < 50; ++seed) {
        std::uint16_t moves, blocked;
        RunResult r = runStrategy<WallFollower>(seed, 0, moves, blocked);
        TEST_ASSERT_EQUAL(25, r.cellsVisited);
        TEST_ASSERT_FALSE(r.strategyErr);
        TEST_ASSERT_EQUAL(0, blocked);
    }
}

void test_wall_follower_terminates_on_looped_mazes() {
    for (std::uint32_t seed = 0; seed < 50; ++seed) {
        std::uint16_t moves, blocked;
        RunResult r = runStrategy<WallFollower>(seed, 8, moves, blocked);
        TEST_ASSERT_FALSE(r.strategyErr);
        TEST_ASSERT_EQUAL(0, blocked);
        TEST_ASSERT_LESS_THAN(MazeSolver::kMaxSteps, r.steps);
    }
}

// ---------------- DFS ----------------

template <typename Strategy>
static void checkCoversEveryMaze() {
    const int loopLevels[] = {0, 8, 16};
    for (int loops : loopLevels)
        for (std::uint32_t seed = 0; seed < 50; ++seed) {
            std::uint16_t moves, blocked;
            RunResult r = runStrategy<Strategy>(seed, loops, moves, blocked);
            TEST_ASSERT_EQUAL(25, r.cellsVisited);
            TEST_ASSERT_FALSE(r.strategyErr);
            TEST_ASSERT_EQUAL(0, blocked);
        }
}

template <typename Strategy>
static void checkTreeBound() {
    for (std::uint32_t seed = 0; seed < 50; ++seed) {
        std::uint16_t moves, blocked;
        runStrategy<Strategy>(seed, 0, moves, blocked);
        TEST_ASSERT_LESS_OR_EQUAL(48, moves);
    }
}

void test_dfs_covers_every_maze()                { checkCoversEveryMaze<DfsExplorer>(); }
void test_dfs_respects_the_tree_bound()          { checkTreeBound<DfsExplorer>(); }
void test_dfs_straight_covers_every_maze()       { checkCoversEveryMaze<DfsStraightFirst>(); }
void test_dfs_straight_respects_the_tree_bound() { checkTreeBound<DfsStraightFirst>(); }

int main(int, char**) {
    UNITY_BEGIN();
    RUN_TEST(test_wall_follower_covers_perfect_mazes);
    RUN_TEST(test_wall_follower_terminates_on_looped_mazes);
    RUN_TEST(test_dfs_covers_every_maze);
    RUN_TEST(test_dfs_respects_the_tree_bound);
    RUN_TEST(test_dfs_straight_covers_every_maze);
    RUN_TEST(test_dfs_straight_respects_the_tree_bound);
    return UNITY_END();
}
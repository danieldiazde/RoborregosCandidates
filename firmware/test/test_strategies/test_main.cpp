// firmware/test/test_strategies/test_main.cpp
#include <unity.h>
#include "MazeGenerator.hpp"
#include "FakeRobotIO.hpp"
#include "MazeSolver.hpp"
#include "WallFollower.hpp"
#include "DfsExplorer.hpp"
#include "DfsStraightFirst.hpp"
#include "FrontierExplorer.hpp"

using namespace maze;

void setUp() {}
void tearDown() {}

static const MotionCosts kFree{1, 1, 1, 1};   // correctness tests, not timing

static GeneratedMaze makeMaze(std::uint32_t seed, std::uint8_t loops) {
    GeneratorConfig c;
    c.seed = seed;
    c.extraOpenings = loops;
    return MazeGenerator(c).generate();
}

// Ground truth for "explored everything": cells reachable from the start
// without ever entering the red tile (entering red ends the round).
static int reachableWithoutRed(const GeneratedMaze& g) {
    bool seen[Maze::kSize][Maze::kSize] = {};
    std::int8_t qx[Maze::kSize * Maze::kSize], qy[Maze::kSize * Maze::kSize];
    int head = 0, tail = 0;

    seen[g.startX][g.startY] = true;
    qx[tail] = g.startX; qy[tail] = g.startY; ++tail;

    while (head < tail) {
        std::int8_t x = qx[head], y = qy[head]; ++head;
        for (int i = 0; i < 4; ++i) {
            Direction d = static_cast<Direction>(i);
            if (!g.maze.canMove(x, y, d)) continue;
            std::int8_t nx = x + deltaX(d), ny = y + deltaY(d);
            if (seen[nx][ny]) continue;
            if (g.maze.cell(nx, ny).color() == TileColor::Red) continue;
            seen[nx][ny] = true;
            qx[tail] = nx; qy[tail] = ny; ++tail;
        }
    }
    return tail;
}

struct Outcome {
    RunResult     result;
    std::uint16_t moves;
    std::uint16_t blocked;
    int           expectedCells;
};

template <typename Strategy>
static Outcome runStrategy(std::uint32_t seed, std::uint8_t loops) {
    GeneratedMaze g = makeMaze(seed, loops);

    FakeRobotIO io(g.maze, g.startX, g.startY, g.startHeading, kFree);
    Strategy strategy;
    MazeSolver solver(io, strategy);

    Outcome o;
    o.result        = solver.run();
    o.moves         = io.moveCount();
    o.blocked       = io.blockedCount();
    o.expectedCells = reachableWithoutRed(g);
    return o;
}

// ---------------- shared checks ----------------

// Explores everything reachable without red, then finishes on red.
template <typename Strategy>
static void checkCoversEverythingExceptRed(const int* loopLevels, int levels) {
    for (int k = 0; k < levels; ++k)
        for (std::uint32_t seed = 0; seed < 50; ++seed) {
            Outcome o = runStrategy<Strategy>(seed, static_cast<std::uint8_t>(loopLevels[k]));
            TEST_ASSERT_EQUAL(o.expectedCells + 1, o.result.cellsVisited);   // + red at the end
            TEST_ASSERT_TRUE(o.result.reachedRed);
            TEST_ASSERT_FALSE(o.result.strategyErr);
            TEST_ASSERT_EQUAL(0, o.blocked);
        }
}

// Exploration drives each tree corridor at most twice: 2 * (cells - 1).
template <typename Strategy>
static void checkTreeBound() {
    for (std::uint32_t seed = 0; seed < 50; ++seed) {
        Outcome o = runStrategy<Strategy>(seed, 0);
        TEST_ASSERT_LESS_OR_EQUAL(2 * (o.expectedCells - 1), o.result.exploreSteps);
    }
}

static const int kPerfectOnly[] = {0};
static const int kAllLoops[]    = {0, 8, 16};

// ---------------- WallFollower ----------------

void test_wall_follower_covers_perfect_mazes() {
    checkCoversEverythingExceptRed<WallFollower>(kPerfectOnly, 1);
}

void test_wall_follower_is_clean_on_looped_mazes() {
    for (std::uint32_t seed = 0; seed < 50; ++seed) {
        Outcome o = runStrategy<WallFollower>(seed, 8);
        TEST_ASSERT_FALSE(o.result.strategyErr);
        TEST_ASSERT_EQUAL(0, o.blocked);
        TEST_ASSERT_LESS_THAN(MazeSolver::kMaxSteps, o.result.steps);
    }
}

// ---------------- DFS (both variants) ----------------

void test_dfs_covers_every_maze()                { checkCoversEverythingExceptRed<DfsExplorer>(kAllLoops, 3); }
void test_dfs_respects_the_tree_bound()          { checkTreeBound<DfsExplorer>(); }
void test_dfs_straight_covers_every_maze()       { checkCoversEverythingExceptRed<DfsStraightFirst>(kAllLoops, 3); }
void test_dfs_straight_respects_the_tree_bound() { checkTreeBound<DfsStraightFirst>(); }

// ---------------- Frontier ----------------

void test_frontier_covers_every_maze() { checkCoversEverythingExceptRed<FrontierExplorer>(kAllLoops, 3); }

// On looped mazes Frontier should need fewer exploration steps than DFS
// on average, because it takes shortcuts instead of retracing its trail.
void test_frontier_beats_dfs_on_looped_mazes() {
    long frontier = 0, dfs = 0;
    for (std::uint32_t seed = 0; seed < 100; ++seed) {
        frontier += runStrategy<FrontierExplorer>(seed, 8).result.exploreSteps;
        dfs      += runStrategy<DfsExplorer>(seed, 8).result.exploreSteps;
    }
    TEST_ASSERT_LESS_THAN(dfs, frontier);
}

int main(int, char**) {
    UNITY_BEGIN();
    RUN_TEST(test_wall_follower_covers_perfect_mazes);
    RUN_TEST(test_wall_follower_is_clean_on_looped_mazes);
    RUN_TEST(test_dfs_covers_every_maze);
    RUN_TEST(test_dfs_respects_the_tree_bound);
    RUN_TEST(test_dfs_straight_covers_every_maze);
    RUN_TEST(test_dfs_straight_respects_the_tree_bound);
    RUN_TEST(test_frontier_covers_every_maze);
    RUN_TEST(test_frontier_beats_dfs_on_looped_mazes);
    return UNITY_END();
}

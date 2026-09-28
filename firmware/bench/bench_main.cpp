// firmware/bench/bench_main.cpp
#include <cstdio>
#include "MazeGenerator.hpp"
#include "MazeValidator.hpp"
#include "FakeRobotIO.hpp"
#include "MazeSolver.hpp"
#include "WallFollower.hpp"
#include "DfsExplorer.hpp"
#include "DfsStraightFirst.hpp"

using namespace maze;

template <typename Strategy>
bool runOne(std::uint32_t seed, int loops) {
    GeneratorConfig c;
    c.seed = seed;
    c.extraOpenings = static_cast<std::uint8_t>(loops);
    GeneratedMaze g = MazeGenerator(c).generate();

    ValidationResult v = validate(g);
    if (!v.ok) {
        std::fprintf(stderr, "invalid maze seed=%u loops=%d: %s\n", seed, loops, v.reason);
        return false;
    }

    FakeRobotIO io(g.maze, g.startX, g.startY, g.startHeading);
    Strategy strategy;
    MazeSolver solver(io, strategy);
    RunResult r = solver.run();

    std::printf("%s,%u,%d,%u,%u,%u,%u,%d,%d,%d,%u\n",
        strategy.name(), seed, loops,
        static_cast<unsigned>(io.moveCount()),
        static_cast<unsigned>(io.turnCount()),
        static_cast<unsigned>(io.elapsedMs()),
        static_cast<unsigned>(r.cellsVisited),
        r.reachedRed, r.timedOut, r.strategyErr,
        static_cast<unsigned>(io.blockedCount()));
    return true;
}

int main() {
    std::printf("strategy,seed,loops,moves,turns,time_ms,cells,"
                "reached_red,timed_out,strategy_err,blocked\n");

    const int loopLevels[] = {0, 4, 8, 12, 16};

    for (std::uint32_t seed = 0; seed < 500; ++seed)
        for (int loops : loopLevels) {
            if (!runOne<WallFollower>(seed, loops))     return 1;
            if (!runOne<DfsExplorer>(seed, loops))      return 1;
            if (!runOne<DfsStraightFirst>(seed, loops)) return 1;
        }

    return 0;
}
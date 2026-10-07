// firmware/lib/maze/MazeSolver.hpp
#pragma once
#include <cstdint>
#include "config.h"
#include "IRobotIO.hpp"
#include "ExplorationStrategy.hpp"

namespace maze {

struct RunResult {
    std::uint16_t steps        = 0;
    std::uint16_t exploreSteps = 0;       // steps before the finishing phase
    std::uint8_t  cellsVisited = 0;
    bool          reachedRed   = false;
    bool          timedOut     = false;
    bool          strategyErr  = false;   // strategy asked for an illegal move
};

class MazeSolver {
public:
    static constexpr std::uint16_t kMaxLog      = config::kMaxMoveLogEntries;
    static constexpr std::uint16_t kMaxSteps    = config::kMaxSolverSteps;
    static constexpr std::uint32_t kDeadlineMs  = config::kSimulatedFinishReserveMs;

    MazeSolver(IRobotIO& io, ExplorationStrategy& strategy);

    RunResult run();

    const Maze&    belief() const;
    Pose           pose() const;
    std::uint16_t  logLength() const;
    Direction      logAt(std::uint16_t i) const;

private:
    void establishStart();
    void observe(RunResult& result);
    void faceTowards(Direction target);
    bool stepToward(Direction d);
    bool findFinishTarget(std::int8_t& tx, std::int8_t& ty) const;

    IRobotIO&            io_;
    ExplorationStrategy& strategy_;
    Maze                 belief_;
    Pose                 pose_{0, 0, Direction::North};
    bool                 finishing_ = false;

    Direction     log_[kMaxLog];
    std::uint16_t logLen_ = 0;
};

}  // namespace maze

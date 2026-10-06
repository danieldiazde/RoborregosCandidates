// Native-only fake used by tests and benchmarks; never include it in target code.
#pragma once

#ifdef ARDUINO
#error "FakeRobotIO is native-only"
#endif

#include "IRobotIO.hpp"
#include "Maze.hpp"
#include "config.h"

namespace maze {

struct MotionCosts {
    std::uint32_t advanceMs    = config::kSimulatedAdvanceMs;
    std::uint32_t strafeMs     = config::kSimulatedStrafeMs;
    std::uint32_t turnMs       = config::kSimulatedTurnMs;
    std::uint32_t turnAroundMs = config::kSimulatedTurnAroundMs;
};

class FakeRobotIO : public IRobotIO {
public:
    FakeRobotIO(const Maze& truth, std::int8_t x, std::int8_t y,
                Direction heading, MotionCosts costs = MotionCosts{});

    WallReading senseWalls() override;
    LineReading senseLine() override;
    TileColor readTile() override;
    Possession possession() const override;
    TileColor readTileAhead() override;

    ActionResult advance() override;
    ActionResult retreat() override;
    ActionResult strafe(Strafe side) override;
    ActionResult turn(RelativeTurn relative) override;
    void stop() override;

    ActionResult grip(GripCommand command) override;
    void showColor(TileColor color) override;
    std::uint32_t millisRemaining() const override;

    void setMillisRemaining(std::uint32_t ms);
    TileColor lastShownColor() const;
    std::int8_t x() const;
    std::int8_t y() const;
    Direction heading() const;
    std::uint16_t moveCount() const;
    std::uint16_t turnCount() const;
    std::uint16_t blockedCount() const;
    std::uint32_t elapsedMs() const;

private:
    ActionResult moveToward(Direction d, std::uint32_t costMs);
    void spend(std::uint32_t ms);

    const Maze& truth_;
    std::int8_t x_;
    std::int8_t y_;
    Direction heading_;
    MotionCosts costs_;
    Possession possession_ = Possession::Empty;
    std::uint32_t millisRemaining_ = config::kSimulatedRoundDurationMs;
    std::uint32_t elapsedMs_ = 0;
    TileColor lastShown_ = TileColor::Unknown;
    std::uint16_t moveCount_ = 0;
    std::uint16_t turnCount_ = 0;
    std::uint16_t blockedCount_ = 0;
};

}  // namespace maze

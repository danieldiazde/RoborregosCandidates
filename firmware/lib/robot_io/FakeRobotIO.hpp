#pragma once
#include "IRobotIO.hpp"
#include "Maze.hpp"

namespace maze {

class FakeRobotIO : public IRobotIO {
public:
    // Constructor
    FakeRobotIO(const Maze& truth, std::int8_t x, std::int8_t y, Direction heading);

    // Sensors
    WallReading senseWalls() override;
    LineReading senseLine() override;
    TileColor readTile() override;
    Possession possession() const override;

    // Motion
    ActionResult advance() override;
    ActionResult retreat() override;
    ActionResult strafe(Strafe side) override;
    ActionResult turn(RelativeTurn relative) override;
    void stop() override;

    // Capture
    ActionResult grip(GripCommand command) override;

    // Output
    void showColor(TileColor color) override;

    // Clock
    std::uint32_t millisRemaining() const override;

private:
    const Maze& truth_;
    std::uint8_t x_;
    std::uint8_t y_;
    Direction heading_;
};


}
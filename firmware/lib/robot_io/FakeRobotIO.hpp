// firmware/lib/robot_io/FakeRobotIO.hpp
#pragma once
#include "IRobotIO.hpp"
#include "Maze.hpp"

namespace maze {

class FakeRobotIO : public IRobotIO {
public:
    FakeRobotIO(const Maze& truth, std::int8_t x, std::int8_t y, Direction heading);

    // Sensors
    WallReading senseWalls() override;
    LineReading senseLine() override;
    TileColor   readTile() override;
    Possession  possession() const override;

    // Motion
    ActionResult advance() override;
    ActionResult retreat() override;
    ActionResult strafe(Strafe side) override;
    ActionResult turn(RelativeTurn relative) override;
    void         stop() override;

    // Capture
    ActionResult grip(GripCommand command) override;

    // Output
    void showColor(TileColor color) override;

    // Clock
    std::uint32_t millisRemaining() const override;

    // Test hooks — not part of IRobotIO
    void          setMillisRemaining(std::uint32_t ms);
    TileColor     lastShownColor() const;
    std::int8_t   x() const;
    std::int8_t   y() const;
    Direction     heading() const;
    std::uint16_t moveCount() const;

private:
    ActionResult moveToward(Direction d);

    const Maze&   truth_;
    std::int8_t   x_;
    std::int8_t   y_;
    Direction     heading_;

    Possession    possession_      = Possession::Empty;
    std::uint32_t millisRemaining_ = 360000;   // 6 minutes
    TileColor     lastShown_       = TileColor::Unknown;
    std::uint16_t moveCount_       = 0;
};

}  // namespace maze
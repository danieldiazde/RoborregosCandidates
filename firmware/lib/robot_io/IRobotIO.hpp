// firmware/lib/robot_io/IRobotIO.hpp
#pragma once
#include <cstdint>
#include "Motion.hpp"
#include "Capture.hpp"
#include "TileColor.hpp"

namespace maze {

class IRobotIO {
public:
    virtual ~IRobotIO() = default;

    // Sensors
    virtual WallReading senseWalls()       = 0;
    virtual LineReading senseLine()        = 0;
    virtual TileColor   readTile()         = 0;
    virtual Possession  possession() const = 0;

    // Motion
    virtual ActionResult advance()                   = 0;
    virtual ActionResult retreat()                   = 0;
    virtual ActionResult strafe(Strafe side)         = 0;
    virtual ActionResult turn(RelativeTurn relative) = 0;
    virtual void         stop()                      = 0;

    // Capture
    virtual ActionResult grip(GripCommand command) = 0;

    // Output
    virtual void showColor(TileColor color) = 0;

    // Clock
    virtual std::uint32_t millisRemaining() const = 0;
};

}  // namespace maze
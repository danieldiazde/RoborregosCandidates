// firmware/lib/robot_io/FakeRobotIO.cpp
#include "FakeRobotIO.hpp"

namespace maze {

FakeRobotIO::FakeRobotIO(const Maze& truth, std::int8_t x, std::int8_t y, Direction heading)
    : truth_(truth), x_(x), y_(y), heading_(heading) {}

// Sensors

WallReading FakeRobotIO::senseWalls() {
    WallReading r;
    r.left  = truth_.wall(x_, y_, turnLeft(heading_))  != WallState::Open;
    r.front = truth_.wall(x_, y_, heading_)            != WallState::Open;
    r.right = truth_.wall(x_, y_, turnRight(heading_)) != WallState::Open;
    r.valid = true;
    return r;
}

LineReading FakeRobotIO::senseLine() {
    LineReading r;
    r.left  = false;
    r.right = false;
    r.valid = true;
    return r;
}

TileColor FakeRobotIO::readTile() {
    return truth_.cell(x_, y_).color();
}

Possession FakeRobotIO::possession() const {
    return possession_;
}

// Motion

ActionResult FakeRobotIO::moveToward(Direction d) {
    if (!truth_.canMove(x_, y_, d)) return ActionResult::Blocked;
    x_ += deltaX(d);
    y_ += deltaY(d);
    ++moveCount_;
    return ActionResult::Ok;
}

ActionResult FakeRobotIO::advance() {
    return moveToward(heading_);
}

ActionResult FakeRobotIO::retreat() {
    return moveToward(opposite(heading_));
}

ActionResult FakeRobotIO::strafe(Strafe side) {
    Direction d = (side == Strafe::Left) ? turnLeft(heading_) : turnRight(heading_);
    return moveToward(d);
}

ActionResult FakeRobotIO::turn(RelativeTurn relative) {
    switch (relative) {
        case RelativeTurn::Left:   heading_ = turnLeft(heading_);  break;
        case RelativeTurn::Right:  heading_ = turnRight(heading_); break;
        case RelativeTurn::Around: heading_ = opposite(heading_);  break;
    }
    return ActionResult::Ok;
}

void FakeRobotIO::stop() {}

// Capture

ActionResult FakeRobotIO::grip(GripCommand command) {
    possession_ = (command == GripCommand::Close) ? Possession::Holding
                                                  : Possession::Empty;
    return ActionResult::Ok;
}

// Output

void FakeRobotIO::showColor(TileColor color) {
    lastShown_ = color;
}

// Clock

std::uint32_t FakeRobotIO::millisRemaining() const {
    return millisRemaining_;
}

// Hooks for tests

void          FakeRobotIO::setMillisRemaining(std::uint32_t ms) { millisRemaining_ = ms; }
TileColor     FakeRobotIO::lastShownColor() const { return lastShown_; }
std::int8_t   FakeRobotIO::x() const { return x_; }
std::int8_t   FakeRobotIO::y() const { return y_; }
Direction     FakeRobotIO::heading() const { return heading_; }
std::uint16_t FakeRobotIO::moveCount() const { return moveCount_; }

}  // namespace maze
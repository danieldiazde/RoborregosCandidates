// firmware/lib/robot_io/FakeRobotIO.cpp
#include "FakeRobotIO.hpp"

namespace maze {

FakeRobotIO::FakeRobotIO(const Maze& truth, std::int8_t x, std::int8_t y, Direction heading, MotionCosts costs)
    : truth_(truth), x_(x), y_(y), heading_(heading), costs_(costs) {}

// Sensors

WallReading FakeRobotIO::senseWalls() {
    WallReading r;
    r.left  = truth_.wall(x_, y_, turnLeft(heading_))  != WallState::Open;
    r.front = truth_.wall(x_, y_, heading_)            != WallState::Open;
    r.right = truth_.wall(x_, y_, turnRight(heading_)) != WallState::Open;
    r.valid = true;
    return r;
}

void FakeRobotIO::spend(std::uint32_t ms) {
    elapsedMs_ += ms;
    millisRemaining_ = (ms >= millisRemaining_) ? 0 : millisRemaining_ - ms;
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

ActionResult FakeRobotIO::moveToward(Direction d, std::uint32_t costMs) {
    if (!truth_.canMove(x_, y_, d)) {
        ++blockedCount_;
        return ActionResult::Blocked;
    }
    x_ += deltaX(d);
    y_ += deltaY(d);
    ++moveCount_;
    spend(costMs);
    return ActionResult::Ok;
}

ActionResult FakeRobotIO::advance() {
    return moveToward(heading_, costs_.advanceMs);
}

ActionResult FakeRobotIO::retreat() {
    return moveToward(opposite(heading_), costs_.advanceMs);
}


ActionResult FakeRobotIO::strafe(Strafe side) {
    Direction d = (side == Strafe::Left) ? turnLeft(heading_) : turnRight(heading_);
    return moveToward(d, costs_.strafeMs);
}

ActionResult FakeRobotIO::turn(RelativeTurn relative) {
    switch (relative) {
        case RelativeTurn::Left:
            heading_ = turnLeft(heading_);
            spend(costs_.turnMs);
            break;
        case RelativeTurn::Right:
            heading_ = turnRight(heading_);
            spend(costs_.turnMs);
            break;
        case RelativeTurn::Around:
            heading_ = opposite(heading_);
            spend(costs_.turnAroundMs);
            break;
    }
    ++turnCount_;
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
std::uint16_t FakeRobotIO::turnCount() const    { return turnCount_; }
std::uint16_t FakeRobotIO::blockedCount() const { return blockedCount_; }
std::uint32_t FakeRobotIO::elapsedMs() const    { return elapsedMs_; }

}  // namespace maze
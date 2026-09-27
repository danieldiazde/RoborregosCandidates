#include "MazeSolver.hpp"

namespace maze {

MazeSolver::MazeSolver(IRobotIO& io, ExplorationStrategy& strategy)
    : io_(io), strategy_(strategy) {}

const Maze&   MazeSolver::belief() const    { return belief_; }
Pose          MazeSolver::pose() const      { return pose_; }
std::uint16_t MazeSolver::logLength() const { return logLen_; }
Direction     MazeSolver::logAt(std::uint16_t i) const { return log_[i]; }

void MazeSolver::establishStart() {
    WallReading r = io_.senseWalls();
    pose_ = r.left ? Pose{0, 0, Direction::North}
                   : Pose{Maze::kSize - 1, 0, Direction::North};
}

void MazeSolver::observe(RunResult& result) {
    WallReading r = io_.senseWalls();
    if (r.valid) {
        auto state = [](bool wall) { return wall ? WallState::Blocked : WallState::Open; };
        belief_.setWall(pose_.x, pose_.y, turnLeft(pose_.heading),  state(r.left));
        belief_.setWall(pose_.x, pose_.y, pose_.heading,            state(r.front));
        belief_.setWall(pose_.x, pose_.y, turnRight(pose_.heading), state(r.right));
    }

    Cell& c = belief_.cell(pose_.x, pose_.y);
    if (!c.isVisited()) {
        c.markVisited();
        ++result.cellsVisited;

        TileColor t = io_.readTile();
        c.setColor(t);
        if (t == TileColor::Red) result.reachedRed = true;
        if (t == TileColor::Cyan || t == TileColor::Yellow ||
            t == TileColor::Orange || t == TileColor::Magenta)
            io_.showColor(t);
    }
}

void MazeSolver::faceTowards(Direction target) {
    int delta = (static_cast<int>(target) - static_cast<int>(pose_.heading) + 4) & 3;
    ActionResult r = ActionResult::Ok;
    switch (delta) {
        case 0: return;
        case 1: r = io_.turn(RelativeTurn::Right);  break;
        case 2: r = io_.turn(RelativeTurn::Around); break;
        case 3: r = io_.turn(RelativeTurn::Left);   break;
    }
    if (r == ActionResult::Ok) pose_.heading = target;
}

bool MazeSolver::stepToward(Direction d) {
    faceTowards(d); 
    if (pose_.heading != d) return false;

    ActionResult r = io_.advance();
    if (r == ActionResult::Blocked) {
        belief_.setWall(pose_.x, pose_.y, pose_.heading, WallState::Blocked);
    }
    if (r != ActionResult::Ok) return false;
    pose_.x += deltaX(pose_.heading);
    pose_.y += deltaY(pose_.heading);

    if (logLen_ < kMaxLog) log_[logLen_++] = pose_.heading;
    return true;

}

RunResult MazeSolver::run() {
    RunResult result;
    establishStart();
    observe(result);

    for (std::uint16_t i = 0; i < kMaxSteps; ++i) {
        if (io_.millisRemaining() < kDeadlineMs) {
            result.timedOut = true; break;
        }
        std::optional<Direction> next = strategy_.next(belief_, pose_);
        if (next == std::nullopt) break;
        if (!belief_.canMove(pose_.x, pose_.y, *next)) {
            result.strategyErr = true; break;
        }
        if (stepToward(*next)) {
            result.steps++;
            observe(result);
        }
    }

    return result;
}

} // namespace maze
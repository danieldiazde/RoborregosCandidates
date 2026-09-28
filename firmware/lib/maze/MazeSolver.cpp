#include "MazeSolver.hpp"
#include "PathPlanner.hpp"

namespace maze {

MazeSolver::MazeSolver(IRobotIO& io, ExplorationStrategy& strategy)
    : io_(io), strategy_(strategy) {}

const Maze&   MazeSolver::belief() const    { return belief_; }
Pose          MazeSolver::pose() const      { return pose_; }
std::uint16_t MazeSolver::logLength() const { return logLen_; }
Direction     MazeSolver::logAt(std::uint16_t i) const { return log_[i]; }

void MazeSolver::establishStart() {
    pose_ = Pose{0, 0, Direction::North};
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

    std::int8_t nx = pose_.x + deltaX(d);
    std::int8_t ny = pose_.y + deltaY(d);

    TileColor ahead = io_.readTileAhead();
    if (ahead != TileColor::Unknown) belief_.cell(nx, ny).setColor(ahead);
    if (ahead == TileColor::Red && !finishing_) return false;   // look, don't step

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
    result.exploreSteps = result.steps;

    // Finishing phase: exploration is over (or time is short) — drive to red.
    finishing_ = true;
    for (std::uint16_t i = 0; i < kMaxSteps && !result.reachedRed; ++i) {
        if (io_.millisRemaining() == 0) break;

        std::int8_t tx, ty;
        if (!findFinishTarget(tx, ty)) break;

        std::optional<Direction> step = firstStepToward(belief_, pose_.x, pose_.y, tx, ty);
        if (!step) break;

        if (stepToward(*step)) {
            result.steps++;
            observe(result);
        }
    }

    return result;
}

// Where to finish: the red tile if we've seen it otherwise the nearest
// unvisited corner we know a path to, since red is always on a corner
bool MazeSolver::findFinishTarget(std::int8_t& tx, std::int8_t& ty) const {
    for (std::int8_t x = 0; x < Maze::kSize; ++x)
        for (std::int8_t y = 0; y < Maze::kSize; ++y)
            if (belief_.cell(x, y).color() == TileColor::Red) {
                tx = x; ty = y;
                return true;
            }

    const std::int8_t last = Maze::kSize - 1;
    const std::int8_t corners[4][2] = {{0, 0}, {last, 0}, {0, last}, {last, last}};
    for (const auto& c : corners) {
        if (belief_.cell(c[0], c[1]).isVisited()) continue;
        if (!firstStepToward(belief_, pose_.x, pose_.y, c[0], c[1])) continue;
        tx = c[0]; ty = c[1];
        return true;
    }
    return false;
}

} // namespace maze
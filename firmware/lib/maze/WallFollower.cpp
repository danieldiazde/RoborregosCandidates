// firmware/lib/maze/WallFollower.cpp
#include "WallFollower.hpp"

namespace maze {

const char* WallFollower::name() const { return "wall_follower"; }

std::optional<Direction> WallFollower::next(const Maze& known, const Pose& pose) {
    const bool moved = !started_ || pose.x != lastX_ || pose.y != lastY_;

    if (moved) {
        arrival_ = pose.heading;
        const int h = static_cast<int>(arrival_);
        if (seen_[pose.x][pose.y][h]) return std::nullopt;   // walk is repeating
        seen_[pose.x][pose.y][h] = true;
    }
    started_ = true;
    lastX_ = pose.x;
    lastY_ = pose.y;

    Direction order[4] = { turnLeft(arrival_), arrival_,
                           turnRight(arrival_), opposite(arrival_) };

    for (int i = 0; i < 4; ++i) {
        Direction d = order[i];
        if (!known.canMove(pose.x, pose.y, d)) continue;
        if (known.cell(pose.x + deltaX(d), pose.y + deltaY(d)).color() == TileColor::Red) continue;
        return d;
    }
    return std::nullopt;
}

}  // namespace maze

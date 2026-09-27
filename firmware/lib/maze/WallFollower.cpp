// firmware/lib/maze/WallFollower.cpp
#include "WallFollower.hpp"

namespace maze {

const char* WallFollower::name() const { return "wall_follower"; }

std::optional<Direction> WallFollower::next(const Maze& known, const Pose& pose) {
    const int h = static_cast<int>(pose.heading);

    if (seen_[pose.x][pose.y][h]) return std::nullopt;
    seen_[pose.x][pose.y][h] = true;

    Direction order[4] = { turnLeft(pose.heading), pose.heading,
                           turnRight(pose.heading), opposite(pose.heading) };

    for (int i = 0; i < 4; ++i)
        if (known.canMove(pose.x, pose.y, order[i])) return order[i];

    return std::nullopt;
}

}  // namespace maze
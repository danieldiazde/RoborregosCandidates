// firmware/lib/maze/DfsExplorer.cpp
#include "DfsExplorer.hpp"

namespace maze {

const char* DfsExplorer::name() const { return "dfs_nesw"; }

void DfsExplorer::order(const Pose&, Direction out[4]) const {
    out[0] = Direction::North;
    out[1] = Direction::East;
    out[2] = Direction::South;
    out[3] = Direction::West;
}

std::optional<Direction> DfsExplorer::next(const Maze& known, const Pose& pose) {
    // The start is the bottom of the stack
    if (top_ == 0) stack_[top_++] = {pose.x, pose.y};

    // Try to go deeper, in whatever order this variant prefers
    Direction candidates[4];
    order(pose, candidates);

    for (int i = 0; i < 4; ++i) {
        Direction d = candidates[i];
        std::int8_t nx = pose.x + deltaX(d);
        std::int8_t ny = pose.y + deltaY(d);
        if (known.canMove(pose.x, pose.y, d) && !known.cell(nx, ny).isVisited() &&  known.cell(nx, ny).color() != TileColor::Red) {
            stack_[top_++] = {nx, ny};
            return d;
        }
    }

    // Dead end
    --top_;

    // Back at the start with nothing left: exploration complete
    if (top_ == 0) return std::nullopt;

    // Step back toward the cell underneath
    Pos parent = stack_[top_ - 1];
    return directionTo(pose.x, pose.y, parent.x, parent.y);
}

}  // namespace maze
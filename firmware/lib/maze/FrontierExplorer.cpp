// firmware/lib/maze/FrontierExplorer.cpp
#include "FrontierExplorer.hpp"
#include "PathPlanner.hpp"

namespace maze {

const char* FrontierExplorer::name() const { return "frontier_bfs"; }

std::optional<Direction> FrontierExplorer::next(const Maze& known, const Pose& pose) {
    return firstStepToNearestUnvisited(known, pose.x, pose.y);
}

}  // namespace maze

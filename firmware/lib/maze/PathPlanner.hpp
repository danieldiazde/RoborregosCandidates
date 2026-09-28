// firmware/lib/maze/PathPlanner.hpp
#pragma once
#include <optional>
#include "Maze.hpp"

namespace maze {

// First step of the shortest known-open path from (fx,fy) to (tx,ty),
// or nullopt if already there or no path is known.
// Never routes *through* a red tile; red is allowed only as the target.
std::optional<Direction> firstStepToward(const Maze& known,
                                         std::int8_t fx, std::int8_t fy,
                                         std::int8_t tx, std::int8_t ty);

// First step toward the nearest cell (by moves) that has not been visited,
// or nullopt if no unvisited cell is reachable through known-open walls.
// Never routes through or targets a red tile.
std::optional<Direction> firstStepToNearestUnvisited(const Maze& known,
                                                     std::int8_t fx, std::int8_t fy);

}  // namespace maze

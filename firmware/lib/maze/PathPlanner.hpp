// firmware/lib/maze/PathPlanner.hpp
#pragma once
#include <optional>
#include "Maze.hpp"

namespace maze {

std::optional<Direction> firstStepToward(const Maze& known,
                                         std::int8_t fx, std::int8_t fy,
                                         std::int8_t tx, std::int8_t ty);

}  // namespace maze
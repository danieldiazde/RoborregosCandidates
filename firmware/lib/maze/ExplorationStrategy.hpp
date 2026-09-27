// firmware/lib/maze/ExplorationStrategy.hpp
#pragma once
#include <optional>
#include "Maze.hpp"
#include "Pose.hpp"

namespace maze {

class ExplorationStrategy {
public:
    virtual ~ExplorationStrategy() = default;
    virtual const char* name() const = 0;
    virtual std::optional<Direction> next(const Maze& known, const Pose& pose) = 0;
};

}  // namespace maze
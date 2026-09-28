// firmware/lib/maze/FrontierExplorer.hpp
#pragma once
#include "ExplorationStrategy.hpp"

namespace maze {

// Always heads for the nearest unvisited cell, using the known map.
// Unlike DFS it never retraces its own trail when a shortcut exists.
class FrontierExplorer : public ExplorationStrategy {
public:
    const char* name() const override;
    std::optional<Direction> next(const Maze& known, const Pose& pose) override;
};

}  // namespace maze

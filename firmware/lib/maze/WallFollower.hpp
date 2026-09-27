// firmware/lib/maze/WallFollower.hpp
#pragma once
#include "ExplorationStrategy.hpp"

namespace maze {

class WallFollower : public ExplorationStrategy {
public:
    const char* name() const override;
    std::optional<Direction> next(const Maze& known, const Pose& pose) override;

private:
    bool seen_[Maze::kSize][Maze::kSize][4] = {};
};

}  // namespace maze
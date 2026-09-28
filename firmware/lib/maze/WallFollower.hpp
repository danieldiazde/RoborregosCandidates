// firmware/lib/maze/WallFollower.hpp
#pragma once
#include "ExplorationStrategy.hpp"

namespace maze {

class WallFollower : public ExplorationStrategy {
public:
    const char* name() const override;
    std::optional<Direction> next(const Maze& known, const Pose& pose) override;

private:
    bool        seen_[Maze::kSize][Maze::kSize][4] = {};
    bool        started_ = false;
    std::int8_t lastX_   = 0;
    std::int8_t lastY_   = 0;
    Direction   arrival_ = Direction::North;   // direction we last travelled in
};

}  // namespace maze

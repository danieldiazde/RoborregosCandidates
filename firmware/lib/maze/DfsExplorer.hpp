// firmware/lib/maze/DfsExplorer.hpp
#pragma once
#include "ExplorationStrategy.hpp"

namespace maze {

class DfsExplorer : public ExplorationStrategy {
public:
    const char* name() const override;
    std::optional<Direction> next(const Maze& known, const Pose& pose) override;

protected:
    virtual void order(const Pose& pose, Direction out[4]) const;

private:
    struct Pos { std::int8_t x; std::int8_t y; };
    Pos stack_[Maze::kSize * Maze::kSize];
    int top_ = 0;
};

}  // namespace maze
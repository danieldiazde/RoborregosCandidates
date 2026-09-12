// firmware/lib/maze/Maze.hpp
#pragma once
#include <cstdint>
#include "Cell.hpp"
#include "Direction.hpp"

namespace maze {

enum class WallState : std::uint8_t { Unknown = 0, Open, Blocked };

class Maze {
public:
    static const std::int8_t kSize = 5;

    Maze();

    bool contains(std::int8_t x, std::int8_t y) const;

    WallState wall(std::int8_t x, std::int8_t y, Direction d) const;
    void      setWall(std::int8_t x, std::int8_t y, Direction d, WallState state);

    bool canMove(std::int8_t x, std::int8_t y, Direction d) const;

    const Cell& cell(std::int8_t x, std::int8_t y) const;
    Cell&       cell(std::int8_t x, std::int8_t y);

private:
    const WallState& wallSlot(std::int8_t x, std::int8_t y, Direction d) const;
    WallState&       wallSlot(std::int8_t x, std::int8_t y, Direction d);

    WallState verticalWalls_[kSize + 1][kSize];
    WallState horizontalWalls_[kSize][kSize + 1];
    Cell      cells_[kSize][kSize];
};

}  // namespace maze
// firmware/lib/maze/Maze.cpp
#include "Maze.hpp"

namespace maze {

Maze::Maze() {
    for (std::int8_t x = 0; x <= kSize; ++x)
        for (std::int8_t y = 0; y < kSize; ++y)
            verticalWalls_[x][y] = WallState::Unknown;

    for (std::int8_t x = 0; x < kSize; ++x)
        for (std::int8_t y = 0; y <= kSize; ++y)
            horizontalWalls_[x][y] = WallState::Unknown;

    // Outer boundary
    for (std::int8_t y = 0; y < kSize; ++y) {
        verticalWalls_[0][y]     = WallState::Blocked;
        verticalWalls_[kSize][y] = WallState::Blocked;
    }
    for (std::int8_t x = 0; x < kSize; ++x) {
        horizontalWalls_[x][0]     = WallState::Blocked;
        horizontalWalls_[x][kSize] = WallState::Blocked;
    }
}

bool Maze::contains(std::int8_t x, std::int8_t y) const {
    return x >= 0 && x < kSize && y >= 0 && y < kSize;
}

const WallState* Maze::slot(std::int8_t x, std::int8_t y, Direction d) const {
    if (!contains(x, y)) return nullptr;
    switch (d) {
        case Direction::North: return &horizontalWalls_[x][y + 1];
        case Direction::South: return &horizontalWalls_[x][y];
        case Direction::East:  return &verticalWalls_[x + 1][y];
        case Direction::West:  return &verticalWalls_[x][y];
    }
    return nullptr;
}

WallState* Maze::slot(std::int8_t x, std::int8_t y, Direction d) {
    return const_cast<WallState*>(
        static_cast<const Maze*>(this)->slot(x, y, d));
}

WallState Maze::wall(std::int8_t x, std::int8_t y, Direction d) const {
    const WallState* s = slot(x, y, d);
    return s ? *s : WallState::Blocked;
}

void Maze::setWall(std::int8_t x, std::int8_t y, Direction d, WallState state) {
    if (WallState* s = slot(x, y, d)) *s = state;
}

bool Maze::canMove(std::int8_t x, std::int8_t y, Direction d) const {
    if (wall(x, y, d) != WallState::Open) return false;
    return contains(x + deltaX(d), y + deltaY(d));
}

const Cell& Maze::cell(std::int8_t x, std::int8_t y) const { return cells_[x][y]; }
Cell& Maze::cell(std::int8_t x, std::int8_t y) { return cells_[x][y]; }

}  // namespace maze
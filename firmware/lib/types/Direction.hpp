#pragma once
#include <cstdint>
#include <optional>

namespace maze {

    enum class Direction : std::uint8_t {North = 0, East = 1, South = 2, West = 3};

Direction turnRight(Direction d);
Direction turnLeft(Direction d);
Direction opposite(Direction d);
std::int8_t deltaX(Direction d);
std::int8_t deltaY(Direction d);
Direction dirAt(int i);
std::optional<Direction> directionTo(std::int8_t fromX, std::int8_t fromY,
                                     std::int8_t toX,   std::int8_t toY);

}
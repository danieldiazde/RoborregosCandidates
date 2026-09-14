#pragma once
#include <cstdint>

namespace maze {

    enum class Direction : std::uint8_t {North = 0, East = 1, South = 2, West = 3};

Direction turnRight(Direction d);
Direction turnLeft(Direction d);
Direction opposite(Direction d);
std::int8_t deltaX(Direction d);
std::int8_t deltaY(Direction d);

}
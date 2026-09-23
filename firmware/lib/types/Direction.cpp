// firmware/lib/types/Direction.cpp
#include "Direction.hpp"

namespace maze {

Direction turnRight(Direction d) { // The & 3 wraps around so a 4 becomes a 0 and a 6 becomes a 2
    return static_cast<Direction>((static_cast<std::uint8_t>(d) + 1) & 3);
}

Direction turnLeft(Direction d) {
    return static_cast<Direction>((static_cast<std::uint8_t>(d) + 3) & 3);
}

Direction opposite(Direction d) {
    return static_cast<Direction>((static_cast<std::uint8_t>(d) + 2) & 3);
}

std::int8_t deltaX(Direction d) {
    switch (d) {
        case Direction::East: return  1;
        case Direction::West: return -1;
        default:              return  0;
    }
}

std::int8_t deltaY(Direction d) {
    switch (d) {
        case Direction::North: return  1;
        case Direction::South: return -1;
        default:               return  0;
    }
}

}  // namespace maze
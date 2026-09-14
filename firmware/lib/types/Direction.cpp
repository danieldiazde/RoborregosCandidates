#include "Direction.hpp"

namespace maze {

std::int8_t deltaX(Direction d) {
    switch (d) {
        case Direction::East: return 1;
        case Direction::West: return -1;
        default: return 0;
    }
}

std::int8_t deltaY(Direction d) {
    switch (d) {
        case Direction::North: return 1;
        case Direction::South: return -1;
        default: return 0;
    }
}

}
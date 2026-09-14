#pragma once
#include <stdint.h>

namespace maze {

enum class Terrain : std::uint8_t {
    Flat = 0,
    SpeedBump,
    Stairs,
    Ramps

};
}
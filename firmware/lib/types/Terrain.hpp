#pragma once
#include <cstdint>

namespace maze {

enum class Terrain : std::uint8_t {
    Flat = 0,
    SpeedBump,
    Stairs,
    Ramps
};
}

// firmware/lib/types/TileColor.hpp
#pragma once
#include <cstdint>

namespace maze {

enum class TileColor : std::uint8_t {
    Unknown = 0,   // hasn't been read
    White,          //  floor
    Cyan,
    Yellow,
    Orange,
    Magenta,
    Green,          // start tile
    Red             // final checkpoint
};

}  // namespace maze
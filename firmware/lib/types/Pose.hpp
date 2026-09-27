// firmware/lib/types/Pose.hpp
#pragma once
#include <cstdint>
#include "Direction.hpp"

namespace  maze
{
    struct Pose {std::int8_t x; std::int8_t y; Direction heading;};
} // namespace  maze

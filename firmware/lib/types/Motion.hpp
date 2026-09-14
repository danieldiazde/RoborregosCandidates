// firmware/lib/types/Action.hpp
#pragma once
#include <cstdint>

namespace maze {

enum class RelativeTurn : std::uint8_t { Left, Right, Around };

enum class Strafe : std::uint8_t {Left, Right};

enum class ActionResult : std::uint8_t {
    Ok,        // completed as requested
    Blocked,   // stopped early
    Aborted,   // SafetySupervisor 
    Failed     // hardware fault, stall, timeout
};

struct WallReading {
    bool left;
    bool front;
    bool right;
    bool valid;   // false if sensor data was bad
};

}  // namespace maze
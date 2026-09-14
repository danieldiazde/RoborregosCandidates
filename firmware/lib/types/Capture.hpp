#pragma once
#include <cstdint>

namespace maze {
    
enum class GripCommand : std::uint8_t { Open, Close };

enum class Possession : std::uint8_t { Empty, Holding, Unknown };

}
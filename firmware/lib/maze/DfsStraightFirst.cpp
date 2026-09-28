// firmware/lib/maze/DfsStraightFirst.cpp
#include "DfsStraightFirst.hpp"

namespace maze {

const char* DfsStraightFirst::name() const { return "dfs_straight_first"; }

void DfsStraightFirst::order(const Pose& pose, Direction out[4]) const {
    out[0] = pose.heading;              // keep going straight
    out[1] = turnLeft(pose.heading);    // one 90° turn
    out[2] = turnRight(pose.heading);   // one 90° turn
    out[3] = opposite(pose.heading);    // 180°, most expensive
}

}  // namespace maze
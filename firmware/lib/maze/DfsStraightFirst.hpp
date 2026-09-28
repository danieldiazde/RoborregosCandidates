// firmware/lib/maze/DfsStraightFirst.hpp
#pragma once
#include "DfsExplorer.hpp"

namespace maze {

class DfsStraightFirst : public DfsExplorer {
public:
    const char* name() const override;

protected:
    void order(const Pose& pose, Direction out[4]) const override;
};

}  // namespace maze
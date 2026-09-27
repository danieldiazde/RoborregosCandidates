// firmware/lib/maze_gen/MazeValidator.hpp
#pragma once
#include "MazeGenerator.hpp"

namespace maze {

struct ValidationResult {
    bool        ok;
    const char* reason;
};

ValidationResult validate(const GeneratedMaze& g);
int              countOpenInteriorWalls(const Maze& m);

}  // namespace maze
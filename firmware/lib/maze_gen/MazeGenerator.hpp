// firmware/lib/maze_gen/MazeGenerator.hpp
#pragma once
#include <cstdint>
#include <random>
#include "Maze.hpp"

namespace maze {

struct GeneratorConfig {
    std::uint32_t seed          = 0;
    std::uint8_t  extraOpenings = 0;
};

struct GeneratedMaze {
    Maze        maze;
    std::int8_t startX;
    std::int8_t startY;
    Direction   startHeading;
};

class MazeGenerator {
public:
    explicit MazeGenerator(GeneratorConfig config);
    GeneratedMaze generate();

private:
    void          closeAllWalls(Maze& m);
    void          carvePassages(Maze& m, std::int8_t sx, std::int8_t sy);
    void          addLoops(Maze& m);
    void          placeTiles(Maze& m, std::int8_t sx, std::int8_t sy);
    std::uint32_t randomBelow(std::uint32_t n);

    GeneratorConfig config_;
    std::mt19937    rng_;
};

}  // namespace maze
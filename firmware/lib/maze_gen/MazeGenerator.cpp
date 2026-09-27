// firmware/lib/maze_gen/MazeGenerator.cpp
#include "MazeGenerator.hpp"

namespace maze {

constexpr std::int8_t N = Maze::kSize;

MazeGenerator::MazeGenerator(GeneratorConfig config) : config_(config), rng_(config.seed) {}

std::uint32_t MazeGenerator::randomBelow(std::uint32_t n) {
    return rng_() % n;
}

GeneratedMaze MazeGenerator::generate() {
    Maze m;

    // Start coordinate, if it is non-zero (i.e. 5) then it is 4, else it is 0
    std::int8_t sx = randomBelow(2) ? N - 1 : 0;
    std::int8_t sy = randomBelow(2) ? N - 1 : 0;

    // Heading towards the maze
    Direction vertical   = (sy == 0) ? Direction::North : Direction::South;
    Direction horizontal = (sx == 0) ? Direction::East  : Direction::West;

    // Choose if it starts moving towards middle or up relative to the robot
    Direction heading  = randomBelow(2) ? vertical : horizontal;

    // Make it a maze
    closeAllWalls(m);
    carvePassages(m, sx, sy);
    addLoops(m);
    placeTiles(m, sx, sy);

    return GeneratedMaze{m, sx, sy, heading};
}

void MazeGenerator::closeAllWalls(Maze& m) {
    for (std::int8_t x = 0; x < N; ++x)
        for (std::int8_t y = 0; y < N; ++y) {
            m.setWall(x, y, Direction::East,  WallState::Blocked);
            m.setWall(x, y, Direction::North, WallState::Blocked);
        }
}

void MazeGenerator::carvePassages(Maze& m, std::int8_t sx, std::int8_t sy) {

    struct Pos {std::int8_t x; std::int8_t y;};

    bool visited[N][N] = {};

    Pos stack[N * N];
    int top = 0;
    // Array instead of a true c++ stack for memory improvement, fixed 5 * 5
    visited[sx][sy] = true;
    stack[top++] = {sx, sy}; //Leave it in 0

    //DFS for carving passasges
    while (top > 0) {
        Pos curr = stack[top - 1];
        Direction options[4];
        int count = 0;
        for (int i = 0; i < 4; ++i) {
            Direction d = static_cast<Direction>(i);
            std::int8_t neighbourX = curr.x + deltaX(d);
            std::int8_t neighbourY = curr.y + deltaY(d);
            if (m.contains(neighbourX, neighbourY) && !visited[neighbourX][neighbourY]) options[count++] = d;
        }
        if (count == 0) {
            --top;
            continue; //It met a dead end so it has to step back
        }
        Direction d = options[randomBelow(count)]; //So it doesnt always pick North
        std::int8_t nx = curr.x + deltaX(d);
        std::int8_t ny = curr.y + deltaY(d);

        m.setWall(curr.x, curr.y, d, WallState::Open);
        visited[nx][ny] = true;
        stack[top++] = {nx, ny};
    }

}

void MazeGenerator::addLoops(Maze& m) {
    struct WallRef {std::int8_t x; std::int8_t y; Direction d;};

    WallRef candidates[40];
    std::int8_t count = 0;

    // Collect blocked interior walls

    for (std::int8_t x = 0; x < N; ++x) {
        for (std::int8_t y = 0; y < N; ++y) {
            if (x < N - 1 && m.wall(x, y, Direction::East) == WallState::Blocked)
                candidates[count++] = {x, y, Direction::East};
            if (y < N - 1 && m.wall(x, y, Direction::North) == WallState::Blocked)
                candidates[count++] = {x, y, Direction::North};
        }
    }

    // Clamp
    std::int8_t n = config_.extraOpenings;
    if (n > count) n = count;

    // Pick without repeats
    for (std::int8_t k = 0; k < n; ++k) {
        std::uint32_t index = randomBelow(count);
        WallRef w = candidates[index];
        m.setWall(w.x, w.y, w.d, WallState::Open);
        --count;
        std::swap(candidates[index], candidates[count]);
    }
}

void MazeGenerator::placeTiles(Maze& m, std::int8_t sx, std::int8_t sy) {
    
    // Make all the tiles white
    for (std::int8_t x = 0; x < N; ++x) {
        for (std::int8_t y = 0; y < N; ++y) {
            m.cell(x, y).setColor(TileColor::White);
        }
    }

    // Make the starting tile green
    m.cell(sx, sy).setColor(TileColor::Green);

    // Choose placement of random red tile
    bool chosen = false;
    std::int8_t ex = randomBelow(2) ? N - 1 : 0;
    std::int8_t ey = randomBelow(2) ? N - 1 : 0;
    while (ex == sx && ey == sy) {
    ex = randomBelow(2) ? N - 1 : 0;
    ey = randomBelow(2) ? N - 1 : 0;
    }
    m.cell(ex, ey).setColor(TileColor::Red);

    // Assign other colors at random
    TileColor candidates[4] = {TileColor::Cyan, TileColor::Yellow, TileColor::Orange, TileColor::Magenta};
    std::int8_t idx = 0;
    while (idx < 4) {
        std::int8_t ax = randomBelow(N);
        std::int8_t ay = randomBelow(N);
        if (m.cell(ax, ay).color() == TileColor::White) m.cell(ax, ay).setColor(candidates[idx++]);
    }
}

}  // namespace maze
// firmware/lib/maze_gen/MazeValidator.cpp
#include "MazeValidator.hpp"

namespace maze {

namespace {

constexpr std::int8_t N = Maze::kSize;

Direction dirAt(int i) { return static_cast<Direction>(i); }

bool isCorner(std::int8_t x, std::int8_t y) {
    return (x == 0 || x == N - 1) && (y == 0 || y == N - 1);
}

int reachableFrom(const Maze& m, std::int8_t sx, std::int8_t sy) {
    bool seen[N][N] = {};
    std::int8_t qx[N * N], qy[N * N];
    int head = 0, tail = 0;

    seen[sx][sy] = true;
    qx[tail] = sx; qy[tail] = sy; ++tail;

    while (head < tail) {
        std::int8_t x = qx[head], y = qy[head]; ++head;
        for (int i = 0; i < 4; ++i) {
            Direction d = dirAt(i);
            if (!m.canMove(x, y, d)) continue;
            std::int8_t nx = x + deltaX(d), ny = y + deltaY(d);
            if (seen[nx][ny]) continue;
            seen[nx][ny] = true;
            qx[tail] = nx; qy[tail] = ny; ++tail;
        }
    }
    return tail;
}

}  // namespace

int countOpenInteriorWalls(const Maze& m) {
    int open = 0;
    for (std::int8_t x = 0; x < N; ++x)
        for (std::int8_t y = 0; y < N; ++y) {
            if (x < N - 1 && m.wall(x, y, Direction::East)  == WallState::Open) ++open;
            if (y < N - 1 && m.wall(x, y, Direction::North) == WallState::Open) ++open;
        }
    return open;
}

ValidationResult validate(const GeneratedMaze& g) {
    const Maze& m = g.maze;

    for (std::int8_t x = 0; x < N; ++x)
        for (std::int8_t y = 0; y < N; ++y)
            for (int i = 0; i < 4; ++i)
                if (m.wall(x, y, dirAt(i)) == WallState::Unknown)
                    return {false, "unknown wall"};

    for (std::int8_t i = 0; i < N; ++i) {
        if (m.wall(0, i, Direction::West)      != WallState::Blocked) return {false, "open west boundary"};
        if (m.wall(N - 1, i, Direction::East)  != WallState::Blocked) return {false, "open east boundary"};
        if (m.wall(i, 0, Direction::South)     != WallState::Blocked) return {false, "open south boundary"};
        if (m.wall(i, N - 1, Direction::North) != WallState::Blocked) return {false, "open north boundary"};
    }

    if (!isCorner(g.startX, g.startY))
        return {false, "start not in a corner"};
    if (!m.contains(g.startX + deltaX(g.startHeading), g.startY + deltaY(g.startHeading)))
        return {false, "start heading points out of the maze"};

    int counts[8] = {};
    for (std::int8_t x = 0; x < N; ++x)
        for (std::int8_t y = 0; y < N; ++y)
            ++counts[static_cast<int>(m.cell(x, y).color())];

    if (counts[static_cast<int>(TileColor::Unknown)] != 0) return {false, "unknown tile color"};
    if (counts[static_cast<int>(TileColor::Green)]   != 1) return {false, "need exactly one green"};
    if (counts[static_cast<int>(TileColor::Red)]     != 1) return {false, "need exactly one red"};
    if (counts[static_cast<int>(TileColor::Cyan)]    != 1) return {false, "need exactly one cyan"};
    if (counts[static_cast<int>(TileColor::Yellow)]  != 1) return {false, "need exactly one yellow"};
    if (counts[static_cast<int>(TileColor::Orange)]  != 1) return {false, "need exactly one orange"};
    if (counts[static_cast<int>(TileColor::Magenta)] != 1) return {false, "need exactly one magenta"};
    if (m.cell(g.startX, g.startY).color() != TileColor::Green)
        return {false, "start tile is not green"};

    if (reachableFrom(m, g.startX, g.startY) != N * N)
        return {false, "not every cell is reachable"};

    return {true, "ok"};
}

}  // namespace maze
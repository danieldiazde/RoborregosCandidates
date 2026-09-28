// firmware/lib/maze/PathPlanner.cpp
#include "PathPlanner.hpp"

namespace maze {

std::optional<Direction> firstStepToward(const Maze& known,
                                         std::int8_t fx, std::int8_t fy,
                                         std::int8_t tx, std::int8_t ty) {
    constexpr int N = Maze::kSize;

    if (fx == tx && fy == ty) return std::nullopt;              

    if (!known.contains(fx, fy) || !known.contains(tx, ty)) return std::nullopt;

    bool        seen[N][N] = {};
    Direction   first[N][N];
    std::int8_t qx[N * N], qy[N * N];
    int head = 0, tail = 0;

    seen[fx][fy] = true;
    qx[tail] = fx; qy[tail] = fy; ++tail;

    while (head < tail) {
        std::int8_t x = qx[head], y = qy[head]; ++head;

        for (int i = 0; i < 4; ++i) {
            Direction d = dirAt(i);
            if (!known.canMove(x, y, d)) continue;

            std::int8_t nx = x + deltaX(d);
            std::int8_t ny = y + deltaY(d);
            if (seen[nx][ny]) continue;

            bool isTarget = (nx == tx && ny == ty);
            if (!isTarget && known.cell(nx, ny).color() == TileColor::Red) continue;

            // Neighbours of the start get their own direction;
            // everything after inherits the first step of its parent.
            first[nx][ny] = (x == fx && y == fy) ? d : first[x][y];
            if (isTarget) return first[nx][ny];

            seen[nx][ny] = true;
            qx[tail] = nx; qy[tail] = ny; ++tail;
        }
    }
    return std::nullopt;   // target not reachable through known-open walls
}

}  // namespace maze
#pragma once
#include <cstdint>

namespace maze {

    enum class TileColor : std::uint8_t { None = 0, Cyan, Yellow, Orange, Magenta};
    enum class Terrain : std::uint8_t { Flat = 0, Stairs, Speedbump, Ramp };


    class Cell {
    private:
        bool visited_ = false;
        std::uint8_t size_ = 30; //cm
        TileColor color_ = TileColor::None;
        Terrain terrain_ = Terrain::Flat;
    public:
        Cell() = default;
        bool isVisited() const;
        void markVisited();

        TileColor color() const;
        void setColor(TileColor color);

        Terrain terrain() const;
        void setTerrain(Terrain terrain);
};

}


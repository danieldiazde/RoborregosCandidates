#pragma once
#include <cstdint>
#include "TileColor.hpp"
#include "Terrain.hpp"

namespace maze {

    class Cell {
    private:
        bool visited_ = false;
        TileColor color_ = TileColor::Unknown;
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

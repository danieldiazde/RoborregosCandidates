#include "Cell.hpp"

namespace maze {

bool Cell::isVisited() const {return visited_;}

void Cell::markVisited() {visited_ = true;}

TileColor Cell::color() const {return color_;}

void Cell::setColor(TileColor color) {
    color_ = color;
}

Terrain Cell::terrain() const {
    return terrain_;
}

void Cell::setTerrain(Terrain terrain) {
    terrain_ = terrain;
}





}
#include "geometry.h"
#include "TileSurface.h"
#include "STLWriter.h"

using namespace geometry;

int main() {
    // Mt Rainier parameters
    constexpr int zoom = 13;
    constexpr int startX = 1323;
    constexpr int numX = 4;
    constexpr int startY = 2884;
    constexpr int numY = 4;
    std::vector<Tile> tiles;
    for (int x = startX; x < startX + numX; x++) {
        for (int y = startY; y < startY + numY; y++) {
            tiles.emplace_back(x, y, zoom);
        }
    }

    BoundingBox bbox = {.west = startX, .east = startX + numX, .north = startY, .south = startY + numY};
    TileSurface surf(tiles, bbox);

    writeSTL(surf, "test.stl");

    return 0;
}

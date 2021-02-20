#include "catch2/catch.hpp"
#include "TileSurface.h"

TEST_CASE("Construction of a tile surface", "[TileSurface]") {
    int zoom = 3;
    int numX = 3;
    int numY = 4;
    std::vector<Tile> tiles;
    for (int x = 0; x < numX; x++) {
        for (int y = 0; y < numY; y++) {
            tiles.emplace_back(x, y, zoom);
        }
    }

    // This won't work if the data isn't valid
    // TODO: Make a unit test where some of the tiles *aren't* valid and verify the TileSurface is still valid
    for (const auto & t : tiles) {
        REQUIRE(t.valid() == true);
    }
    auto surf = TileSurface(tiles);
    CHECK(surf.size() == numX * numY * TILE_DIM_X * TILE_DIM_Y);
    for (const auto & point : surf) {
        auto uninitialized = (point.lat == 0.0) && (point.lon == 0.0) && (point.alt == 0.0);
        CHECK(uninitialized == false);
    }
}
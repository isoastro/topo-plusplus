#include "catch2/catch.hpp"
#include "TileSurface.h"

TEST_CASE("Construction of a tile surface", "[TileSurface]") {
    constexpr int zoom = 3;
    constexpr int startX = 1;
    constexpr int numX = 3;
    constexpr int startY = 2;
    constexpr int numY = 2;
    std::vector<Tile> tiles;
    for (int x = startX; x < startX + numX; x++) {
        for (int y = startY; y < startY + numY; y++) {
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
        auto uninitialized = std::isnan(point.lat) || std::isnan(point.lon) || std::isnan(point.alt);
        CHECK(uninitialized == false);
    }
}
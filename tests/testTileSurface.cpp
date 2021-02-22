#include "catch2/catch.hpp"
#include "TileSurface.h"
#include <tuple>

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

    BoundingBox bbox = {.west = startX, .east = startX + numX, .north = startY, .south = startY + numY};

    // This won't work if the data isn't valid
    // TODO: Make a unit test where some of the tiles *aren't* valid and verify the TileSurface is still valid
    for (const auto & t : tiles) {
        REQUIRE(t.valid() == true);
    }
    auto surf = TileSurface(tiles, bbox);
    CHECK(surf.size() == numX * numY * TILE_DIM_X * TILE_DIM_Y);
    CHECK(surf.numLat() == numY * TILE_DIM_Y);
    CHECK(surf.numLon() == numX * TILE_DIM_X);

    // Check every single point in the created surface for:
    // - No point is uninitialized
    // - Latitude decreases from north to south
    // - Latitude does not change within a row
    // - Longitude increases from west to east
    // - Longitude does not change within a row
    auto [lastLat, lastLon, ignoreme] = surf.at(0, 0);
    for (size_t x = 0; x < surf.numLon(); x++) {
        // This point is the top of the meridian
        // Reset lastLat as we work our way down the meridian in the next loop
        auto [lastLat, lon, ignoreme] = surf.at(x, 0);
        if (x != 0) {
            CHECK(lon > lastLon);
            lastLon = lon;
        }
        for (size_t y = 0; y < surf.numLat(); y++) {
            auto point = surf.at(x, y);
            auto uninitialized = std::isnan(point.lat) || std::isnan(point.lon) || std::isnan(point.alt);
            CHECK(uninitialized == false);

            CHECK(point.lon == surf.at(x, 0).lon); // longitude is the same for all y
            CHECK(point.lat == surf.at(0, y).lat); // latitude is the same for all x

            if (y != 0) {
                CHECK(point.lat < lastLat);
                lastLat = point.lat;
            }
        }
    }
}
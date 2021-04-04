#include "catch2/catch.hpp"
#include "testCommon.h"
#include "TileSurface.h"
#include "TileSurfaceTest.h"
#include <tuple>

using namespace geometry;

TEST_CASE("Meridian correction (tile space)", "[TileSurface]") {
    SECTION("x == west") {
        for (int zoom = 0; zoom < 15; zoom++) {
            for (int x = 0; x < Tile::N(zoom); x++) {
                CHECK(TileSurfaceTest::correctMeridianTile(x, x, zoom) == 0);
            }
        }
    }

    SECTION("2, 15, 4") {
        // N = 2 ^ 4 = 16
        // 15 == 0, 0 == 1, 1 == 2, 2 == 3, ...
        CHECK(TileSurfaceTest::correctMeridianTile(2, 15, 4) == 3);
    }

    SECTION("2, 15, 5") {
        // N = 2 ^ 5 = 32
        // 15 == 0, 16 == 1, ..., 31 == 16, 0 == 17, 1 == 18, 2 == 19
        CHECK(TileSurfaceTest::correctMeridianTile(2, 15, 5) == 19);
    }

    SECTION("15, 2, 4") {
        // N = 2 ^ 4 == 16
        // 2 == 0, 3 == 1, ..., 15 == 13, 0 == 14, 1 == 15
        CHECK(TileSurfaceTest::correctMeridianTile(15, 2, 4) == 13);
    }
}

TEST_CASE("Meridian correction (radians)", "[TileSurface]") {
    SECTION("x == west") {
        for (int frac = 0; frac < 16; frac++) {
            double x = ((M_PI * 2.0) / 16.0) * static_cast<double>(frac) - M_PI;
            double corrected = TileSurfaceTest::correctMeridianLon(x, x);
            compareReals(corrected, -M_PI);
        }
    }

    SECTION("x = 3pi/4, west=pi/2") {
        double corrected = TileSurfaceTest::correctMeridianLon(3.0 * M_PI_4, M_PI_2);
        compareReals(corrected, -M_PI + M_PI_4);
    }

    SECTION("x = pi/2, west=3pi/4") {
        double corrected = TileSurfaceTest::correctMeridianLon(M_PI_2, 3.0 * M_PI_4);
        compareReals(corrected, M_PI - M_PI_4);
    }
}

TEST_CASE("Construction of a tile surface", "[TileSurface]") {
    constexpr int zoom = 6;
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
    auto [lastLon, lastLat, ignoreme] = surf.at(0, 0);
    for (size_t x = 0; x < surf.numLon(); x++) {
        // This point is the top of the meridian
        // Reset lastLat as we work our way down the meridian in the next loop
        auto [lon, lastLat, ignoreme] = surf.at(x, 0);
        if (x != 0) {
            CHECK(lon > lastLon);
            lastLon = lon;
        }
        for (size_t y = 0; y < surf.numLat(); y++) {
            auto point = surf.at(x, y);
            auto uninitialized = std::isnan(point.x) || std::isnan(point.y) || std::isnan(point.z);
            CHECK(uninitialized == false);

            CHECK(point.x == surf.at(x, 0).x); // longitude is the same for all y
            CHECK(point.y == surf.at(0, y).y); // latitude is the same for all x

            if (y != 0) {
                CHECK(point.y < lastLat);
                lastLat = point.y;
            }
        }
    }
}
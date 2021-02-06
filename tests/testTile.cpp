#include "catch2/catch.hpp"
#include "Tile.h"
#include "testCommon.h"

TEST_CASE("Get Tile", "[Tile]") {
    auto t = new Tile(5, 5, 10);
}

TEST_CASE("RGB to meters", "[Tile]") {
    double res;
    SECTION("minimum") {
        res = Tile::rgbToMeters(0, 0, 0);
        compareReals(res, -32768.0);
    }
    SECTION("maximum") {
        res = Tile::rgbToMeters(0xFF, 0xFF, 0xFF);
        compareReals(res, 32767.9960938);
    }
    SECTION("max R") {
        res = Tile::rgbToMeters(0xFF, 0, 0);
        compareReals(res, 32512.0);
    }
    SECTION("max G") {
        res = Tile::rgbToMeters(0, 0xFF, 0);
        compareReals(res,  -32513.0);
    }
    SECTION("max B") {
        res = Tile::rgbToMeters(0, 0, 0xFF);
        compareReals(res, -32767.0039063);
    }
}
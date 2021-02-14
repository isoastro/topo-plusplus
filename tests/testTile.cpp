#include <iostream>
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
        compareReals(res, 32767.99609375);
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
        compareReals(res, -32767.00390625);
    }
}

TEST_CASE("XY coords to LatLon", "[Tile]") {
    LatLon res;
    SECTION("zero x/y") {
        // Always the same coordinate regardless of zoom level (upper left of world map)
        for (int zoom = 0; zoom < 15; zoom++) {
            res = Tile::coordsToLatLon(0, 0, zoom);
            compareReals(res.lat, +1.4844222297453323);
            compareReals(res.lon, -M_PI);
        }
    }
    SECTION("bottom right") {
        for (int zoom = 0; zoom < 15; zoom++) {
            int x = static_cast<int>(std::pow(2, zoom));
            int y = x;
            res = Tile::coordsToLatLon(x, y, zoom);
            compareReals(res.lat, -1.4844222297453323);
            compareReals(res.lon, M_PI);
        }
    }
    SECTION("center of map") {
        for (int zoom = 1; zoom < 15; zoom++) {
            int x = static_cast<int>(std::pow(2, zoom)) / 2;
            int y = x;
            res = Tile::coordsToLatLon(x, y, zoom);
            compareReals(res.lat, 0.0);
            compareReals(res.lon, 0.0);
        }
    }
    SECTION("x = 11, y= 19, zoom = 5") {
        res = Tile::coordsToLatLon(11, 19, 5);
        compareReals(res.lat, -0.55767043418493578);
        compareReals(res.lon, -0.98174770424681057);
    }
}
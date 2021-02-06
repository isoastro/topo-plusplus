#ifndef _Tile_H_
#define _Tile_H_

#include <cstddef>
#include <cstdint>
#include <string>
#include <array>
#include "geometry.h"

constexpr std::size_t TILE_DIM_X = 256;
constexpr std::size_t TILE_DIM_Y = 256;
constexpr std::size_t PNG_CHANNELS = 3;


// Storage container for raw elevation data, in Web Mercator projection
class Tile {
public:
    Tile(int x, int y, int zoom, bool cache=true);
    ~Tile() = default;

    bool valid() const { return m_valid; }

    // TODO: Could be cool to add static methods to construct from a filename string?

    static std::string coordsToURL(int x, int y, int zoom);
    static double rgbToMeters(uint8_t r, uint8_t g, uint8_t b);
    static LatLon coordsToLatLon(double x, double y, int zoom);

private:
    int m_x;
    int m_y;
    int m_zoom;

    bool m_valid = false;

    double m_elevation[TILE_DIM_Y][TILE_DIM_X] = {{0}};
    Point m_data[TILE_DIM_Y][TILE_DIM_X] = {{{}}};
};

#endif // _Tile_H_

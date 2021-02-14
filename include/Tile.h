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

// Storage container for raw elevation data, in Web Mercator projection, as sourced from a map tile server
// https://en.wikipedia.org/wiki/Tiled_web_map
class Tile {
public:
    Tile(int x, int y, int zoom, bool cache=true);
    ~Tile() = default;

    int x() const { return m_x; }
    int y() const { return m_y; }
    int zoom() const { return m_zoom; }
    bool valid() const { return m_valid; }

    LatLon upperLeft() const; // Upper left corner of the tile
    const std::array<LLA, TILE_DIM_X> & row(size_t idx) const { return m_data[idx]; }

    // TODO: Could be cool to add static methods to construct from a filename string?

    static std::string coordsToURL(int x, int y, int zoom);
    static double rgbToMeters(uint8_t r, uint8_t g, uint8_t b);
    static LatLon coordsToLatLon(double x, double y, int zoom);

private:
    int m_x;
    int m_y;
    int m_zoom;

    bool m_valid = false;

    std::array<std::array<LLA, TILE_DIM_X>, TILE_DIM_Y> m_data;
};

#endif // _Tile_H_

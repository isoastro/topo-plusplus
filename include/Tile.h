#ifndef _Tile_H_
#define _Tile_H_

#include <cstddef>
#include <cstdint>
#include <string>
#include <array>

constexpr std::size_t TILE_DIM_X = 256;
constexpr std::size_t TILE_DIM_Y = 256;
constexpr std::size_t PNG_CHANNELS = 3;

using Pixel = std::array<uint8_t, 3>;

class Tile {
public:
    Tile(int x, int y, int zoom, bool cache=true);
    ~Tile() = default;

    bool valid() const { return m_valid; }

    // TODO: Could be cool to add static methods to construct from a filename string?

protected:
    static std::string coordsToURL(int x, int y, int zoom);
    static float rgbToMeters(uint8_t r, uint8_t g, uint8_t b);

private:
    int m_x;
    int m_y;
    int m_zoom;

    bool m_valid = false;

    Pixel m_pixels[TILE_DIM_Y][TILE_DIM_X] = {{{0}}};
};

#endif // _Tile_H_

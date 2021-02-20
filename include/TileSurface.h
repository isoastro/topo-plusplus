#ifndef _TileSurface_H_
#define _TileSurface_H_

#include "Surface.h"
#include "Tile.h"

class TileSurface : public Surface {
public:
    template<class Container>
    explicit TileSurface(const Container & tiles) {
        // Do a first pass through the container to determine the x/y bounding box of the tiles
        // This means we know where to put tiles into the data and avoids having to reallocate as we go
        // TODO: However, this gets complicated if the x coordinate (longitude) crosses the +180/-180 crossing
        // TODO: Need to make an unambiguous designation of east vs west
        // At least I don't have to worry about pole crossing with Web Mercator data...
        // TODO: What to do if one of the tiles is invalid (doesn't have data)?
        // TODO: How to identify and handle holes in the input data
        // TODO: What to do if the tiles are at different zoom levels?
        const Tile & first = tiles[0];
        int minX = first.x();
        int maxX = first.x();
        int minY = first.y();
        int maxY = first.y();
        for (const Tile & t : tiles) {
            int x = t.x();
            int y = t.y();
            if (x < minX) minX = x;
            if (x > maxX) maxX = x;
            if (y < minY) minY = y;
            if (y > maxY) maxY = y;
        }

        constexpr LLA fillVal = {std::numeric_limits<double>::quiet_NaN(),
                                 std::numeric_limits<double>::quiet_NaN(),
                                 std::numeric_limits<double>::quiet_NaN()};
        m_numLon = ((maxX - minX) + 1) * TILE_DIM_X;
        m_numLat = ((maxY - minY) + 1) * TILE_DIM_Y;
        m_data.resize(m_numLat * m_numLon, fillVal);

        // Do the second pass to place the tiles in the correct locations
        for (const Tile & t : tiles) {
            size_t x = (t.x() - minX) * TILE_DIM_X;
            size_t y = (t.y() - minY) * TILE_DIM_Y;

            // Each row is contiguous, but the rows aren't necessarily next to each other in the overall surface
            for (size_t i = 0; i < TILE_DIM_Y; i++) {
                const auto & row = t.row(i);
                size_t idx = ((y + i) * m_numLon) + x;
                std::copy(row.begin(), row.end(), m_data.begin() + idx);
            }
        }
    }
};

#endif // _TileSurface_H_
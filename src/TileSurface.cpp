#include "TileSurface.h"
#include "geometry.h"
#include <algorithm>

template<class Container>
TileSurface::TileSurface(const Container & tiles) {
    // Do a first pass through the container to determine the x/y bounding box of the tiles
    // This means we know where to put tiles into the data and avoids having to reallocate as we go
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

    m_numLat = ((maxX - minX) + 1) * TILE_DIM_X;
    m_numLon = ((maxY - minY) + 1) * TILE_DIM_Y;
    m_data.resize(m_numLat * m_numLon);

    // Do the second pass to place the tiles in the correct locations
    for (const Tile & t : tiles) {
        size_t x = (t.x() - minX) * TILE_DIM_X;
        size_t y = (t.y() - minY) * TILE_DIM_Y;

        // Each row is contiguous, but the rows aren't necessarily next to each other in the overall surface
        for (size_t i = 0; i < TILE_DIM_Y; i++) {
            const auto & row = t.row(i);
            size_t idx = (y * m_numLat) + x + (i * m_numLat);
            std::copy(row.begin(), row.end(), m_data.begin() + idx);
        }
    }
}
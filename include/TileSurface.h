#ifndef _TileSurface_H_
#define _TileSurface_H_

#include "Surface.h"
#include "Tile.h"
#include <algorithm>

class TileSurface : public Surface {
public:
    template<class Container>
    explicit TileSurface(const Container & tiles, const geometry::BoundingBox & bbox) {
        // Do a first pass through the container to determine the location of the tiles in the overall bounding box
        // This means we know where to put tiles into the data and avoids having to reallocate as we go
        // Bounding box coordinates are in x/y tile-space
        // TODO: What to do if one of the tiles is invalid (doesn't have data)?
        // TODO: How to identify and handle holes in the input data
        // TODO: What to do if the tiles are at different zoom levels?
        const Tile & first = tiles[0];

        int westX = static_cast<int>(bbox.west);
        int zoom = first.zoom();
        double westLon = Tile::coordsToLatLon(bbox.west, bbox.north, zoom).lon;

        int minX = correctMeridianTile(first.x(), westX, zoom);
        int maxX = minX;
        int minY = first.y();
        int maxY = minY;
        for (const Tile & t : tiles) {
            if (!t.valid()) {
                continue;
            }
            int x = correctMeridianTile(t.x(), westX, zoom);
            int y = t.y();
            if (x < minX) minX = x;
            if (x > maxX) maxX = x;
            if (y < minY) minY = y;
            if (y > maxY) maxY = y;
        }

        constexpr geometry::Vec3 fillVal = {std::numeric_limits<double>::quiet_NaN(),
                                            std::numeric_limits<double>::quiet_NaN(),
                                            std::numeric_limits<double>::quiet_NaN()};
        m_numLon = ((maxX - minX) + 1) * TILE_DIM_X;
        m_numLat = ((maxY - minY) + 1) * TILE_DIM_Y;
        m_data.resize(m_numLat * m_numLon, fillVal);

        // Do the second pass to place the tiles in the correct locations
        for (const Tile & t : tiles) {
            if (!t.valid()) {
                continue;
            }
            size_t x = (correctMeridianTile(t.x(), westX, zoom) - minX) * TILE_DIM_X;
            size_t y = (t.y() - minY) * TILE_DIM_Y;

            // Each row is contiguous, but the rows aren't necessarily next to each other in the overall surface
            for (size_t i = 0; i < TILE_DIM_Y; i++) {
                const auto & row = t.row(i);
                std::array<geometry::Vec3, TILE_DIM_X> correctedRow = {};
                std::transform(row.begin(), row.end(), correctedRow.begin(),
                    [westLon](const geometry::Vec3 & point) -> geometry::Vec3 {
                        return {.x = correctMeridianLon(point.x, westLon),
                                .y = point.y,
                                .z = point.z};
                });

                size_t idx = ((y + i) * m_numLon) + x;
                std::copy(correctedRow.begin(), correctedRow.end(), m_data.begin() + idx);
            }
        }
    }

protected:
    // Given a tile location in x, the westernmost point in the bounding box, and a zoom level, determine the location
    // (in x) within the bounding box
    // Example:
    // correctMeridianTile(0, 0, z) -> 0
    // correctMeridianTile(2, 15, 4) -> 3 -- because 15 == 0, 0 == 1, 1 == 2, 2 == 3...
    static int correctMeridianTile(int x, int west, int zoom);

    // Given a longitude position and the westernmost point in the bounding box, determine the equivalent longitude
    // treating the westernmost point as -180 degrees, effectively rotating the entire region around the Earth z-axis
    static double correctMeridianLon(double lon, double west);
};

#endif // _TileSurface_H_
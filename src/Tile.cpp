#include <string>
#include <iostream>
#include <cmath>
#include "Tile.h"
#include "cpr/cpr.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

constexpr char URL_HEAD[] = "http://s3.amazonaws.com/elevation-tiles-prod/terrarium/";

Tile::Tile(int x, int y, int zoom, bool cache) : m_x(x), m_y(y), m_zoom(zoom) {
    std::string url = coordsToURL(x, y, zoom);

    cpr::Response r = cpr::Get(cpr::Url{url});
    if (r.status_code != 200) {
        std::cerr << "Failed to get pixel data from " << url << " (returned " << r.status_code << ")" << std::endl;
        return; // TODO: Throw?
    }

    if (cache) {
        // TODO: save image to disk
        // TODO: try loading image from disk *first*
    }

    int imageX = 0;
    int imageY = 0;
    int channels = 0;
    stbi_uc * res = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(r.text.c_str()), r.text.size(),
                                          &imageX, &imageY, &channels, 0);
    if (imageX != TILE_DIM_X || imageY != TILE_DIM_Y || channels != PNG_CHANNELS) {
        std::cerr << "Image failed to read or is incorrect size (" << imageX << ", " << imageY << ", " << channels << ")" << std::endl;
    } else {
        for (std::size_t i = 0; i < TILE_DIM_Y; i++) {
            for (std::size_t j = 0; j < TILE_DIM_X; j++) {
                auto idx = j + i * TILE_DIM_X;
                auto elev = rgbToMeters(
                    res[idx * PNG_CHANNELS + 0],
                    res[idx * PNG_CHANNELS + 1],
                    res[idx * PNG_CHANNELS + 2]
                );
                LatLon ll = coordsToLatLon(static_cast<double>(x) + j / static_cast<double>(TILE_DIM_X),
                                           static_cast<double>(y) + i / static_cast<double>(TILE_DIM_Y),
                                           zoom);
                m_data[i][j] = {ll.lon, ll.lat, elev};
            }
        }

        m_valid = true;
    }

    stbi_image_free(res);
}

std::string Tile::coordsToURL(int x, int y, int zoom) {
    std::ostringstream url;
    url << URL_HEAD;
    url << zoom << "/";
    url << x << "/";
    url << y << ".png";

    return url.str();
}

double Tile::rgbToMeters(uint8_t r, uint8_t g, uint8_t b) {
    return (
        static_cast<double>(r) * 256.0 +
        static_cast<double>(g) +
        static_cast<double>(b) / 256.0 -
        32768.0
        );
}

LatLon Tile::coordsToLatLon(double x, double y, int zoom) {
    auto n = static_cast<double>(N(zoom));
    return {
        .lat = std::atan(std::sinh(M_PI * (1.0 - 2.0 * y / n))),
        .lon = x / n * (2.0 * M_PI) - M_PI,
    };
}

XY Tile::latLonToCoords(double lat, double lon, int zoom) {
    auto n = static_cast<double>(N(zoom));
    return {
        .x = n * ((lon + M_PI) / (2.0 * M_PI)),
        .y = n * (1.0 - (std::log(std::tan(lat) + secant(lat)) / M_PI)) / 2.0,
    };
}

BoundingBox Tile::boundingBox() const {
    auto northwest = coordsToLatLon(static_cast<double>(m_x),
                                    static_cast<double>(m_y),
                                    m_zoom);
    auto southeast = coordsToLatLon(static_cast<double>(m_x + 1),
                                    static_cast<double>(m_y + 1),
                                    m_zoom);
    return {
        .west = northwest.lon,
        .east = southeast.lon,
        .north = northwest.lat,
        .south = southeast.lat,
    };
}

#include <string>
#include <iostream>
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
                m_pixels[i][j] = {
                    res[idx * PNG_CHANNELS + 0],
                    res[idx * PNG_CHANNELS + 1],
                    res[idx * PNG_CHANNELS + 2],
                };
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

float Tile::rgbToMeters(uint8_t r, uint8_t g, uint8_t b) {
    return (
        static_cast<float>(r) * 256.0f +
        static_cast<float>(g) +
        static_cast<float>(b) / 256.0f -
        32768.0f
        );
}



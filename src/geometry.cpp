#include "geometry.h"
#include <cmath>

double geometry::secant(double x) {
    return 1.0 / std::cos(x);
}

geometry::Point::Point(const geometry::LLA & sph) {
    double cosLat = std::cos(sph.lat);
    double sinLat = std::sin(sph.lat);

    double C = 1.0 / std::sqrt(square(cosLat) + FF_WGS84 * square(sinLat));
    double S = C * FF_WGS84;

    x = (EARTH_RADIUS_WGS84 * C + sph.alt) * cosLat * std::cos(sph.lon);
    y = (EARTH_RADIUS_WGS84 * C + sph.alt) * cosLat * std::sin(sph.lon);
    z = (EARTH_RADIUS_WGS84 * S + sph.alt) * sinLat;
}
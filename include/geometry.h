#ifndef _geometry_H_
#define _geometry_H_

#include <cmath>

constexpr double EARTH_RADIUS_WGS84 = 6378137.0;
constexpr double INV_FLATTENING_FACTOR_WGS84 = 298.257223563;

constexpr double square(double x) {
    return x * x;
}
constexpr double FF_WGS84 = square(1.0 - (1.0 / INV_FLATTENING_FACTOR_WGS84));

struct LatLon {
    double lat;
    double lon;
};

struct LLA {
    double lat;
    double lon;
    double alt;
};

struct Point {
    double x;
    double y;
    double z;

    Point() : x(0), y(0), z(0) {}

    // Construct a point from spherical coordinates to ECEF using the WGS84 model
    explicit Point(const LLA & sph) {
        double cosLat = std::cos(sph.lat);
        double sinLat = std::sin(sph.lat);

        double C = 1.0 / std::sqrt(square(cosLat) + FF_WGS84 * square(sinLat));
        double S = C * FF_WGS84;

        x = (EARTH_RADIUS_WGS84 * C + sph.alt) * cosLat * std::cos(sph.lon);
        y = (EARTH_RADIUS_WGS84 * C + sph.alt) * cosLat * std::sin(sph.lon);
        z = (EARTH_RADIUS_WGS84 * S + sph.alt) * sinLat;
    }
};

#endif // _geometry_H_

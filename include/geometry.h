#ifndef _geometry_H_
#define _geometry_H_

#include <cmath>

namespace geometry {

double secant(double x);

constexpr double EARTH_RADIUS_WGS84 = 6378137.0;
constexpr double INV_FLATTENING_FACTOR_WGS84 = 298.257223563;

// constexpr version of std::pow(2, x)
constexpr double square(double x) {
    return x * x;
}

constexpr double FF_WGS84 = square(1.0 - (1.0 / INV_FLATTENING_FACTOR_WGS84));

struct XY {
    double x;
    double y;
};

struct LatLon {
    double lat;
    double lon;
};

struct BoundingBox {
    double west; // For gridded data, this is the *lowest* x-coord data
    double east; // For gridded data, this is the *highest* x-coord data
    double north; // For gridded data, this is the *lowest* y-coord data
    double south; // For gridded data, this is the *highest* y-coord data

    [[nodiscard]] LatLon northwest() const { return {north, west}; }
    [[nodiscard]] LatLon southwest() const { return {south, west}; }
    [[nodiscard]] LatLon northeast() const { return {north, east}; }
    [[nodiscard]] LatLon southeast() const { return {south, east}; }
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
    explicit Point(const LLA & sph);
};

} // namespace geometry
#endif // _geometry_H_

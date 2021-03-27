#ifndef _geometry_H_
#define _geometry_H_

#include <cmath>

namespace geometry {

double secant(double x);

// TODO: Move out of geometry into Earth-related namespace or something
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

// TODO: Consolidate LLA and Point into a single Vec3 or something, otherwise I may find myself duplicating functionality
// between the two structs
struct LLA {
    double lat;
    double lon;
    double alt;
};

struct Point {
    double x;
    double y;
    double z;

    Point(double x, double y, double z) : x(x), y(y), z(z) {}
    Point() : Point(0, 0, 0) {}

    // Construct a point from spherical coordinates to ECEF using the WGS84 model
    explicit Point(const LLA & sph);

    // Treating the points as vectors, compute the dot or cross product
    // TODO: The name of this struct maybe a little confusing in this case, may be worth while to follow other standards
    // and go with Vec3 or something
    [[nodiscard]] double dot(const Point & other) const;
    [[nodiscard]] Point cross(const Point & other) const;

    // 2-norm (Euclidean norm) - square root of sum of squares
    [[nodiscard]] double norm() const;

    // Unary and binary point operations
    Point operator-() const; // Negation
    Point & operator+=(const Point & rhs); // Addition
    Point & operator-=(const Point & rhs); // Subtraction
    Point & operator*=(double rhs); // Multiplication (scaling)
    Point & operator/=(double rhs); // Division (scaling)
};

// Binary operations
Point operator+(Point lhs, const Point & rhs);
Point operator-(Point lhs, const Point & rhs);
Point operator*(Point lhs, double rhs);
Point operator/(Point lhs, double rhs);

// Construct a unit normal vector from the three points provided
// Segments are AB, then BC (right hand rule)
Point normal(const Point & a, const Point & b, const Point & c);

} // namespace geometry
#endif // _geometry_H_

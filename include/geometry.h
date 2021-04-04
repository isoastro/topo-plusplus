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

struct Vec3 {
    double x;
    double y;
    double z;

    // Treating as vectors, compute the dot or cross product
    [[nodiscard]] double dot(const Vec3 & other) const;
    [[nodiscard]] Vec3 cross(const Vec3 & other) const;

    // 2-norm (Euclidean norm) - square root of sum of squares
    [[nodiscard]] double norm() const;

    // Unary and binary operations
    Vec3 operator-() const; // Negation
    Vec3 & operator+=(const Vec3 & rhs); // Addition
    Vec3 & operator-=(const Vec3 & rhs); // Subtraction
    Vec3 & operator*=(double rhs); // Multiplication (scaling)
    Vec3 & operator/=(double rhs); // Division (scaling)
};

// Binary operations
Vec3 operator+(Vec3 lhs, const Vec3 & rhs);
Vec3 operator-(Vec3 lhs, const Vec3 & rhs);
Vec3 operator*(Vec3 lhs, double rhs);
Vec3 operator/(Vec3 lhs, double rhs);

// Construct a unit normal vector from the three points provided
// Segments are AB, then BC (right hand rule)
Vec3 normal(const Vec3 & a, const Vec3 & b, const Vec3 & c);

} // namespace geometry
#endif // _geometry_H_

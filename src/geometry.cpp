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

geometry::Point geometry::Point::operator-() const {
    return { -this->x, -this->y, -this->z };
}

geometry::Point & geometry::Point::operator+=(const geometry::Point & rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    return *this;
}

geometry::Point operator+(geometry::Point lhs, const geometry::Point & rhs) {
    lhs += rhs;
    return lhs;
}

geometry::Point & geometry::Point::operator-=(const geometry::Point & rhs) {
    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;
    return *this;
}

geometry::Point operator-(geometry::Point lhs, const geometry::Point & rhs) {
    lhs -= rhs;
    return lhs;
}

geometry::Point & geometry::Point::operator*=(double rhs) {
    this->x *= rhs;
    this->y *= rhs;
    this->z *= rhs;
    return *this;
}

geometry::Point operator*(geometry::Point lhs, double rhs) {
    lhs *= rhs;
    return lhs;
}

geometry::Point & geometry::Point::operator/=(double rhs) {
    this->x /= rhs;
    this->y /= rhs;
    this->z /= rhs;
    return *this;
}

geometry::Point operator/(geometry::Point lhs, double rhs) {
    lhs /= rhs;
    return lhs;
}

double geometry::Point::dot(const geometry::Point & other) const {
    return (this->x * other.x) + (this->y * other.y) + (this->z * other.z);
}

geometry::Point geometry::Point::cross(const geometry::Point & other) const {
    return {this->y * other.z - this->z * other.y,
            this->z * other.x - this->x * other.z,
            this->x * other.y - this->y * other.x,};
}

double geometry::Point::norm() const {
    return std::sqrt(this->dot(*this));
}

geometry::Point geometry::normal(const geometry::Point & a, const geometry::Point & b, const geometry::Point & c) {
    auto crossProduct = (b - a).cross(c - b);
    return crossProduct / crossProduct.norm();
}
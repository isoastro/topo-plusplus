#include "geometry.h"
#include <cmath>

double geometry::secant(double x) {
    return 1.0 / std::cos(x);
}

geometry::Vec3 geometry::Vec3::fromECEF(const geometry::Vec3 & sph) {
    double cosLat = std::cos(sph.y);
    double sinLat = std::sin(sph.y);

    double C = 1.0 / std::sqrt(square(cosLat) + FF_WGS84 * square(sinLat));
    double S = C * FF_WGS84;

    return {
        .x = (EARTH_RADIUS_WGS84 * C + sph.z) * cosLat * std::cos(sph.x),
        .y = (EARTH_RADIUS_WGS84 * C + sph.z) * cosLat * std::sin(sph.x),
        .z = (EARTH_RADIUS_WGS84 * S + sph.z) * sinLat,
    };
}

geometry::Vec3 geometry::Vec3::operator-() const {
    return { -this->x, -this->y, -this->z };
}

geometry::Vec3 & geometry::Vec3::operator+=(const geometry::Vec3 & rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    return *this;
}

geometry::Vec3 geometry::operator+(geometry::Vec3 lhs, const geometry::Vec3 & rhs) {
    lhs += rhs;
    return lhs;
}

geometry::Vec3 & geometry::Vec3::operator-=(const geometry::Vec3 & rhs) {
    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;
    return *this;
}

geometry::Vec3 geometry::operator-(geometry::Vec3 lhs, const geometry::Vec3 & rhs) {
    lhs -= rhs;
    return lhs;
}

geometry::Vec3 & geometry::Vec3::operator*=(double rhs) {
    this->x *= rhs;
    this->y *= rhs;
    this->z *= rhs;
    return *this;
}

geometry::Vec3 geometry::operator*(geometry::Vec3 lhs, double rhs) {
    lhs *= rhs;
    return lhs;
}

geometry::Vec3 & geometry::Vec3::operator/=(double rhs) {
    this->x /= rhs;
    this->y /= rhs;
    this->z /= rhs;
    return *this;
}

geometry::Vec3 geometry::operator/(geometry::Vec3 lhs, double rhs) {
    lhs /= rhs;
    return lhs;
}

double geometry::Vec3::dot(const geometry::Vec3 & other) const {
    return (this->x * other.x) + (this->y * other.y) + (this->z * other.z);
}

geometry::Vec3 geometry::Vec3::cross(const geometry::Vec3 & other) const {
    return {this->y * other.z - this->z * other.y,
            this->z * other.x - this->x * other.z,
            this->x * other.y - this->y * other.x,};
}

double geometry::Vec3::norm() const {
    return std::sqrt(this->dot(*this));
}

geometry::Vec3 geometry::normal(const geometry::Vec3 & a, const geometry::Vec3 & b, const geometry::Vec3 & c) {
    auto crossProduct = (b - a).cross(c - b);
    double norm = crossProduct.norm();
    if (norm <= std::numeric_limits<decltype(norm)>::epsilon()) {
        return {0, 0, 1};
    }
    return crossProduct / norm;
}

std::ostream & geometry::operator<<(std::ostream & os, const geometry::Vec3 & v) {
    auto val = static_cast<float>(v.x);
    os.write(reinterpret_cast<const char *>(&val), sizeof(val));
    val = static_cast<float>(v.y);
    os.write(reinterpret_cast<const char *>(&val), sizeof(val));
    val = static_cast<float>(v.z);
    os.write(reinterpret_cast<const char *>(&val), sizeof(val));

    return os;
}

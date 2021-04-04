#ifndef  _STLWRITER_H_
#define  _STLWRITER_H_

#include <iostream>
#include <cstdint>
#include <string>
#include "geometry.h"
#include "Surface.h"

constexpr size_t STL_HEADER_BYTES = 80;

struct STLHeader {
    uint8_t header[STL_HEADER_BYTES];
    uint32_t numTriangles;
};

struct STLTriangle {
    geometry::Vec3 normal{};
    geometry::Vec3 vertex1{};
    geometry::Vec3 vertex2{};
    geometry::Vec3 vertex3{};
    uint16_t attributeByteCount = 0;

    STLTriangle(const geometry::Vec3 & v1, const geometry::Vec3 & v2, const geometry::Vec3 & v3) {
        vertex1 = v1;
        vertex2 = v2;
        vertex3 = v3;
        normal = geometry::normal(v1, v2, v3);
    }
};

std::ostream & operator<<(std::ostream & os, const STLHeader & header);
std::ostream & operator<<(std::ostream & os, const STLTriangle & tri);

void writeSTL(const Surface & surface, const std::string & filename);

#endif // _STLWRITER_H_

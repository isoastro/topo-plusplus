#include <fstream>
#include "STLWriter.h"
#include "Surface.h"

using namespace geometry;

std::ostream & operator<<(std::ostream & os, const STLHeader & header) {
    os.write(reinterpret_cast<const char *>(header.header), sizeof(header.header));
    os.write(reinterpret_cast<const char *>(&header.numTriangles), sizeof(header.numTriangles));
    return os;
}

std::ostream & operator<<(std::ostream & os, const STLTriangle & tri) {
    os << tri.normal << tri.vertex1 << tri.vertex2 << tri.vertex3;
    os.write(reinterpret_cast<const char *>(&tri.attributeByteCount), sizeof(tri.attributeByteCount));
    return os;
}

void writeSTL(const Surface & surface, const std::string & filename) {
    std::ofstream f(filename, std::ios::out | std::ios::binary);

    const size_t numLat = surface.numLat();
    const size_t numLon = surface.numLon();

    STLHeader header{};
    header.numTriangles = static_cast<uint32_t>((numLat - 1) * (numLon - 1) * 2);
    f << header;

    size_t numTrisWritten = 0;
    for (size_t lat = 0; lat < numLat - 1; lat++) {
        for (size_t lon = 0; lon < numLon - 1; lon++) {
            // A B
            // C D

            // TODO: This is unrotated, need to transform so its pointing upward
            // TODO: I don't need to do this transformation AGAIN for A and C because I did them earlier at the previous longitude
            //       Cache the results and save some cycles
            //       Alternatively, apply the transformation to the data up front. The downside there is that the transformation
            //       is destructive...
            auto A = Vec3::fromECEF(surface.at(lon, lat));
            auto B = Vec3::fromECEF(surface.at(lon + 1, lat));
            auto C = Vec3::fromECEF(surface.at(lon, lat + 1));
            auto D = Vec3::fromECEF(surface.at(lon + 1, lat + 1));

            STLTriangle top(B, A, C);
            STLTriangle bottom(C, D, B);
            f << top;
            f << bottom;
            numTrisWritten += 2;
        }
    }

    std::cout << numTrisWritten << "\n";

    f.close();
}
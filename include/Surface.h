#ifndef _Surface_H_
#define _Surface_H_

#include <vector>
#include "geometry.h"

using namespace geometry;

class Surface {
public:
    // Apply a function to every point in the surface -- can transform lat, lon, and/or alt
    template <class UnaryFunction>
    void apply(UnaryFunction f);

    // Scale each elevation point by a given factor (to exaggerate elevation, for example)
    void scaleAlt(double factor);

    [[nodiscard]] size_t numLat() const { return m_numLat; }
    [[nodiscard]] size_t numLon() const { return m_numLon; }

    // Expose the underlying data
    // Thou shalt only iterate constly
    [[nodiscard]] auto begin() const { return m_data.cbegin(); }
    [[nodiscard]] auto end() const { return m_data.cend(); }
    [[nodiscard]] auto rbegin() const { return m_data.crbegin(); }
    [[nodiscard]] auto rend() const { return m_data.crend(); }
    [[nodiscard]] auto size() const { return m_data.size(); }
    [[nodiscard]] const LLA & at(size_t x, size_t y) const;
protected:
    [[nodiscard]] size_t subToIdx(size_t x, size_t y) const;

    std::vector<LLA> m_data = {};

    // Overall dimensions in lat/lon
    // m_data.size() == m_numLat * m_numLon
    size_t m_numLat = 0;
    size_t m_numLon = 0;

    BoundingBox m_boundingBox = {0, 0, 0, 0};
};

#endif // _Surface_H_

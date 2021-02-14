#ifndef _Surface_H_
#define _Surface_H_

#include <vector>
#include "geometry.h"

class Surface {
public:
    // Apply a function to every point in the surface -- can transform lat, lon, or alt
    template <class UnaryFunction>
    void apply(UnaryFunction f);

    // Scale each elevation point by a given factor (to exaggerate elevation, for example)
    void scale(double factor);
protected:
    std::vector<LLA> m_data = {};

    // Overall dimensions in lat/lon
    // m_data.size() == m_numLat * m_numLon
    size_t m_numLat = 0;
    size_t m_numLon = 0;
};

#endif // _Surface_H_

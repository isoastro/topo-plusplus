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

    size_t numLat() const { return m_numLat; }
    size_t numLon() const { return m_numLon; }

    std::vector<LLA>::iterator begin() { return m_data.begin(); }
    std::vector<LLA>::iterator end() { return m_data.end(); }
    std::vector<LLA>::const_iterator cbegin() const { return m_data.cbegin(); }
    std::vector<LLA>::const_iterator cend() const { return m_data.cend(); }
    std::vector<LLA>::reverse_iterator rbegin() { return m_data.rbegin(); }
    std::vector<LLA>::reverse_iterator rend() { return m_data.rend(); }
    std::vector<LLA>::const_reverse_iterator crbegin() const { return m_data.crbegin(); }
    std::vector<LLA>::const_reverse_iterator crend () const { return m_data.crend(); }

    size_t size() { return m_data.size(); }
protected:
    std::vector<LLA> m_data = {};

    // Overall dimensions in lat/lon
    // m_data.size() == m_numLat * m_numLon
    size_t m_numLat = 0;
    size_t m_numLon = 0;
};

#endif // _Surface_H_

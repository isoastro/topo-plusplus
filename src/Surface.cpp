#include "Tile.h"
#include "Surface.h"
#include <algorithm>
#include <stdexcept>

template<class UnaryFunction>
void Surface::apply(UnaryFunction f) {
    std::transform(m_data.begin(), m_data.end(), m_data.begin(), f);
}

void Surface::scaleAlt(double factor) {
    apply([factor](const LLA & point) -> LLA {
        return {
            .lat = point.lat,
            .lon = point.lon,
            .alt = point.alt * factor,
        };
    });
}

const LLA & Surface::at(size_t x, size_t y) const {
    auto idx = subToIdx(x, y);
    if (idx > m_data.size()) {
        throw std::out_of_range("Attempt to access point out of range");
    }
    return m_data[idx];
}

size_t Surface::subToIdx(size_t x, size_t y) const {
    return (y * m_numLon) + x;
}

#include "Tile.h"
#include "Surface.h"
#include <algorithm>
#include <stdexcept>

template<class UnaryFunction>
void Surface::apply(UnaryFunction f) {
    std::transform(m_data.begin(), m_data.end(), m_data.begin(), f);
}

void Surface::scaleAlt(double factor) {
    apply([factor](const Vec3 & point) -> Vec3 {
        return {
            .x = point.x,
            .y = point.y,
            .z = point.z * factor,
        };
    });
}

const Vec3 & Surface::at(size_t x, size_t y) const {
    auto idx = subToIdx(x, y);
    if (idx > m_data.size()) {
        throw std::out_of_range("Attempt to access point out of range");
    }
    return m_data[idx];
}

size_t Surface::subToIdx(size_t x, size_t y) const {
    return (y * m_numLon) + x;
}

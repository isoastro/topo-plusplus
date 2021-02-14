#include "Tile.h"
#include "Surface.h"
#include <algorithm>

template<class UnaryFunction>
void Surface::apply(UnaryFunction f) {
    std::transform(m_data.begin(), m_data.end(), m_data.begin(), f);
}

void Surface::scale(double factor) {
    apply([factor](const LLA & point) -> LLA {
        return {
            .lat = point.lat,
            .lon = point.lon,
            .alt = point.alt * factor,
        };
    });
}
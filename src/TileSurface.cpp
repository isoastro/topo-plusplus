#include "TileSurface.h"
#include <cmath>

int TileSurface::correctMeridianTile(int x, int west, int zoom) {
    auto n = Tile::N(zoom);
    return ((x + n) - west) % n;
}

double TileSurface::correctMeridianLon(double lon, double west) {
    constexpr double twoPi = (M_PI * 2.0);
    double res = std::fmod(lon - west, twoPi) - M_PI;
    if (res >= M_PI) res -= twoPi;
    else if (res < -M_PI) res += twoPi;
    return res;
}

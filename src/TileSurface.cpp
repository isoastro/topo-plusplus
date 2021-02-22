#include "TileSurface.h"
#include "geometry.h"
#include <algorithm>
#include <cmath>

int TileSurface::correctMeridianTile(int x, int west, int zoom) {
    int n = static_cast<int>(std::pow(2.0, zoom));
    return (x - west) % n;
}

double TileSurface::correctMeridianLon(double lon, double west) {
    return std::fmod(lon - west, 2.0 * M_PI) - M_PI;
}

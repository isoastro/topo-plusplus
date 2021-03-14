#ifndef _TileSurfaceTest_H_
#define _TileSurfaceTest_H_

#include "TileSurface.h"

// Expose normally protected methods for use in unit tests
class TileSurfaceTest : public TileSurface {
public:
    using TileSurface::correctMeridianTile;
    using TileSurface::correctMeridianLon;
};

#endif // _TileSurfaceTest_H_

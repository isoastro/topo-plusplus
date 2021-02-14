#ifndef _TileSurface_H_
#define _TileSurface_H_

#include "Surface.h"
#include "Tile.h"

class TileSurface : public Surface {
public:
    template <class Container>
    explicit TileSurface(const Container & tiles);
};

#endif // _TileSurface_H_
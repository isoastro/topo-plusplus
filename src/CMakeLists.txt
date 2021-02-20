add_executable(topo main.cpp)

# Tile utilities
add_library(Tile STATIC Tile.cpp ../include/Tile.h)
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/whoshuu/cpr.git
    GIT_TAG 1.5.2
)
FetchContent_MakeAvailable(cpr)
target_link_libraries(Tile PRIVATE cpr::cpr)

# 3d surface container
add_library(Surface STATIC Surface.cpp)

# Collection of tiles --> Surface
add_library(TileSurface INTERFACE)
target_sources(TileSurface INTERFACE ../include/TileSurface.h)
target_link_libraries(TileSurface INTERFACE Tile Surface)

target_link_libraries(topo
    Tile
    Surface
    TileSurface
)
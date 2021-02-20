add_executable(topo main.cpp)

add_library(geometry SHARED geometry.cpp)
target_include_directories(geometry PUBLIC ../include)

# Tile utilities
add_library(Tile SHARED Tile.cpp)
target_include_directories(Tile PUBLIC ../include)
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/whoshuu/cpr.git
    GIT_TAG 1.5.2
)
FetchContent_MakeAvailable(cpr)
target_link_libraries(Tile
    PRIVATE cpr::cpr
    PUBLIC geometry)

# 3d surface container
add_library(Surface SHARED Surface.cpp)
target_include_directories(Surface PUBLIC ../include)
target_link_libraries(Surface
    PUBLIC geometry)

# Collection of tiles --> Surface
add_library(TileSurface INTERFACE)
target_sources(TileSurface INTERFACE ../include/TileSurface.h)
target_link_libraries(TileSurface INTERFACE Tile Surface)

target_link_libraries(topo
    Tile
    Surface
    TileSurface
)
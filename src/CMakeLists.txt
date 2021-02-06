add_executable(topo main.cpp)

# Tile utilities
add_library(Tile Tile.cpp)
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/whoshuu/cpr.git
    GIT_TAG 1.5.2
)
FetchContent_MakeAvailable(cpr)
target_link_libraries(Tile PRIVATE cpr::cpr)

target_link_libraries(topo Tile)
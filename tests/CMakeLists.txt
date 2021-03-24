# Get Catch2
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v2.13.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(tests
#    testCommon.h
    testMain.cpp
    testTile.cpp
    testTileSurface.cpp
    testGeometry.cpp
)
add_dependencies(tests Tile TileSurface)
target_include_directories(tests PUBLIC . ../include)

target_link_libraries(tests
    PRIVATE Catch2::Catch2
    PUBLIC Tile
           Surface
           TileSurface
           geometry
)

add_test(NAME tests COMMAND tests)
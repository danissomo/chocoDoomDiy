add_executable(diyDoom main.cpp)

add_library(Map_lib STATIC map.cpp ViewRenderer.cpp)
add_library(Wad_lib STATIC  wad_loader.cpp wad_reader.cpp )
add_library(Game_lib STATIC Player.cpp DoomEngine.cpp Game.cpp Angle.cpp )

target_link_libraries(Wad_lib Map_lib)
target_link_libraries(Game_lib Wad_lib Map_lib SDL2)
target_link_libraries(diyDoom Game_lib)

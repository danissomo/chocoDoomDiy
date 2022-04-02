set(CPPS 
"wad_loader.cpp"
"wad_reader.cpp"
"map.cpp"
"ViewRenderer.cpp"
"Angle.cpp"
"Player.cpp"
"DoomEngine.cpp"
"Game.cpp"
"Angle.cpp"
"DisplayManager.cpp"
"Patch.cpp"
"AssetsManager.cpp"
"Weapon.cpp"
"Texture.cpp")

add_executable(diyDoom main.cpp ${CPPS})
target_link_libraries(diyDoom SDL2)


# add_library(Map_lib STATIC map.cpp ViewRenderer.cpp Angle.cpp)
# add_library(Wad_lib STATIC  wad_loader.cpp wad_reader.cpp )
# add_library(Game_lib STATIC Player.cpp DoomEngine.cpp Game.cpp Angle.cpp DisplayManager.cpp Patch.cpp AssetsManager.cpp)

# target_link_libraries(Wad_lib Map_lib)
# target_link_libraries(Game_lib Wad_lib Map_lib SDL2)
# target_link_libraries(diyDoom Game_lib)
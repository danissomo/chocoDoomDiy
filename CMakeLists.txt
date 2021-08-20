cmake_minimum_required(VERSION 3.10.2)
project(diyDoom )
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	add_definitions(-DDEBUG_BUILD)
endif()
add_executable(diyDoom main.cpp)
add_subdirectory(wad_loader)
target_link_libraries(diyDoom wad_loader)



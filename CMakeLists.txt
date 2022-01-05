cmake_minimum_required(VERSION 3.10.2)
project(diyDoom )
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	add_definitions(-DDEBUG_BUILD)
endif()
include_directories("include/")
add_subdirectory(src)



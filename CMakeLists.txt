cmake_minimum_required (VERSION 3.0)

set (PROJECT shade)

# set project name
project (${PROJECT})

# include necessary submodules
list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
)

#set the default path for output
#set(EXECUTABLE_OUTPUT_PATH ${radar_SOURCE_DIR}/build)
#set(LIBRARY_OUTPUT_PATH ${radar_SOURCE_DIR}/build)

# TinyXML
#find_package(TinyXML REQUIRED)

# OpenCV
# find_package(OpenCV REQUIRED )

include(FindFreetype)
include_directories(${FREETYPE_INCLUDE_DIRS})

# boost
#set(Boost_USE_STATIC_LIBS        OFF) # only find static libs
#set(Boost_USE_MULTITHREADED      ON)
##set(Boost_USE_STATIC_RUNTIME    OFF)

#find_package(Boost 1.54.0 COMPONENTS lexical_cast property_tree date_time log filesystem system)

# pkg-config
include (FindPkgConfig)

# gl
pkg_check_modules (GLEW glew)

find_package(GLFW3 REQUIRED)

# message(STATUS "opencv: ${OpenCV_LIBS}")

message(STATUS " GLFW: ${GLFW_INCLUDE_DIRS}")
message(STATUS " GLFW: ${GLFW_LIBRARIES}")

message(STATUS " GLEW: ${GLEW_INCLUDE_DIRS}")
message(STATUS " GLEW: ${GLEW_LIBRARIES}")

add_definitions(-c -pipe -O0 -g -Wall)

include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
#include_directories(${TINYXML_INCLUDE_DIRS})

#if(Boost_FOUND)
#   	include_directories(${Boost_INCLUDE_DIRS})
#endif()

# set search include path
include_directories(".")


# The project version number.
set(VERSION_MAJOR   0   CACHE STRING "Project major version number.")
set(VERSION_MINOR   0   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   1   CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

# build

add_definitions(-std=c++11)

set (HEADERS
./shader.h
./texture.h
./font.h
./buffer.h
./mesh.h
./resource_manager.h
./render.h
./node.h
./camera.h
./font_render.h
./rect_render.h
./line_render.h
./glm_includes.h
./vertex.h
)


set (SOURCES
./shader.cpp
./texture.cpp
./font.cpp
./buffer.cpp
./mesh.cpp
./resource_manager.cpp
./render.cpp
./node.cpp
./camera.cpp
./font_render.cpp
./framebuffer.cpp
)

add_library(${PROJECT} SHARED ${SOURCES})

add_executable (test-${PROJECT} test-shade.cpp ${SOURCES})

target_link_libraries (test-${PROJECT} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} freeimage freetype SOIL)

add_custom_target(clean-all
    COMMAND rm -f Makefile
    COMMAND rm -f cmake_install.cmake
    COMMAND rm -f install_manifest.txt
    COMMAND rm -f CMakeCache.txt
    COMMAND rm -R -f CMakeFiles
)

add_custom_target(install-all
    COMMAND sudo mkdir -p /usr/include/shade/
    COMMAND cp ${HEADERS} /usr/include/shade/
    COMMAND cp ./lib${PROJECT}.so /usr/lib/
)

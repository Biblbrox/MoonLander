cmake_minimum_required(VERSION 3.16)
project(MoonLander)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 20)
set(HPP include/models/ship.hpp include/types.hpp include/utils.hpp include/models/level.hpp
        include/freelable.hpp include/texture.hpp include/window.hpp
        include/geometry.hpp include/models/model.hpp include/timer.h
        include/texttexture.h include/sprite.h include/2dvector.h
        include/game.h)

set(CPP src/main.cpp src/utils.cpp src/level.cpp src/texture.cpp src/window.cpp
        src/geometry.cpp src/ship.cpp src/timer.cpp
        src/texttexture.cpp src/sprite.cpp src/game.cpp)

set(SOURCES ${HPP} ${CPP})
set(SDL2_TTF_LIBRARIES SDL2_ttf)

add_executable(MoonLander ${SOURCES})
target_link_libraries(MoonLander ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}  glfw GLEW libGLEW.so libGLU.so libGL.so )

target_include_directories(MoonLander PRIVATE include)

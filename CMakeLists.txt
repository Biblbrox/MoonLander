cmake_minimum_required(VERSION 3.16)
project(MoonLander)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

add_compile_options("-D_FORTIFY_SOURCE=2" "-D_GLIBCXX_ASSERTIONS" "-fasynchronous-unwind-tables"
        "-fexceptions" "-fpie" "-Wl,-pie" "-fpic" "-shared" "-fstack-clash-protection" "-fstack-protector"
        "-fstack-protector-strong" "-g" "-grecord-gcc-switches" "-fcf-protection" "-pipe" "-Wall" "-Werror=format-security"
        "-Wl,-z,defs" "-Wl,-z,now" "-Wl,-z,relro" "-std=c++2a")

add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 20)
set(HPP include/types.hpp include/utils.hpp include/models/levelgenerator.hpp
        include/texture.hpp include/camera.hpp
        include/geometry.hpp include/timer.hpp
        include/texttexture.hpp include/sprite.hpp include/moonlanderprogram.hpp
        include/game.hpp include/abstract/shaderprogram.hpp
        include/ces/world.hpp include/ces/component.hpp include/ces/entity.hpp include/ces/system.hpp
        include/systems/renderersystem.hpp include/systems/movementsystem.hpp
        include/systems/animationsystem.hpp include/systems/collisionsystem.hpp
        include/systems/physics_system.h)

set(CPP src/main.cpp src/utils.cpp src/levelgenerator.cpp src/texture.cpp
        src/geometry.cpp src/timer.cpp src/moonlanderprogram.cpp  src/camera.cpp
        src/texttexture.cpp src/sprite.cpp src/game.cpp src/shaderprogram.cpp
        src/ces/component.cpp src/ces/system.cpp src/ces/world.cpp src/ces/entity.cpp
        src/systems/renderersystem.cpp src/systems/movementsystem.cpp
        src/systems/keyboardsystem.cpp src/systems/animationsystem.cpp
        src/systems/collisionsystem.cpp src/systems/physicssystem.cpp)

set(SOURCES ${HPP} ${CPP})
set(SDL2_TTF_LIBRARIES SDL2_ttf)

add_executable(MoonLander ${SOURCES})
target_link_libraries(MoonLander ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES} GLEW libGLEW.so libGLU.so libGL.so )

target_include_directories(MoonLander PRIVATE include)

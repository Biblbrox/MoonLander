cmake_minimum_required(VERSION 3.16)

set(GAME_NAME "ECS")
project(${GAME_NAME})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-D_FORTIFY_SOURCE=2" "-D_GLIBCXX_ASSERTIONS"
        "-fasynchronous-unwind-tables" "-fexceptions" "-fpie" "-Wl,-pie" "-fpic"
        "-shared" "-fstack-clash-protection" "-fstack-protector"
        "-fstack-protector-strong" "-g" "-grecord-gcc-switches" "-fcf-protection"
        "-pipe" "-Wall" "-Werror=format-security" "-Wl,-z,defs" "-Wl,-z,now"
        "-O0" "-Wl,-z,relro" "-std=c++17" "-fno-inline-functions")

set(CMAKE_CXX_FLAGS_RELEASE_INIT "-fasynchronous-unwind-tables" "-fexceptions"
        "-fpie" "-Wl,-pie" "-fpic" "-shared" "-fstack-clash-protection"
        "-fstack-protector" "-fstack-protector-strong" "-fcf-protection" "-pipe"
        "-Wall" "-Werror=format-security" "-Wl,-z,defs" "-Wl,-z,now" "-Wl,-z,relro"
        "-O3" "-std=c++17")

set(EXT_PATH "cmake_modules/")

set(CMAKE_CXX_STANDARD 20)
FILE(GLOB_RECURSE CPP RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
FILE(GLOB_RECURSE HPP RELATIVE ${CMAKE_SOURCE_DIR} "include/*.hpp" "include/*.h")

set(SOURCES ${HPP} ${CPP})

add_executable(${GAME_NAME} ${SOURCES})
target_link_libraries(${GAME_NAME})

target_include_directories(${GAME_NAME} PRIVATE include)

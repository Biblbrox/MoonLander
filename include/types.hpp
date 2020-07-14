#ifndef MOONLANDER_TYPES_HPP
#define MOONLANDER_TYPES_HPP

#include <string>

// Declare type for coordinates
typedef int c_type;

//Declare type for velocity
typedef double v_type;

// types of surfaces like Moon, etc...
enum class SurfaceType {
    MOON
};

const std::string RESOURCE_PATH = "../res/";

const std::string GAME_NAME = "Moon Lander";

#endif //MOONLANDER_TYPES_HPP
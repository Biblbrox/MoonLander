#ifndef MOONLANDER_CONSTANTS_HPP
#define MOONLANDER_CONSTANTS_HPP

#include <string>

const GLfloat gravity_force = 0.5f;
const GLfloat weight = 40.f;
const GLfloat engine_force = 1.f;
const GLfloat rot_step = 0.004f;

// types of surfaces like Moon, etc...
enum class SurfaceType
{
    MOON
};

const std::string RESOURCE_PATH = "../res/";
const std::string SHADER_PATH = "../src/shaders/";

const std::string GAME_NAME = "Moon Lander";

#endif //MOONLANDER_CONSTANTS_HPP
#ifndef MOONLANDER_LEVEL_HPP
#define MOONLANDER_LEVEL_HPP

#include <SDL_render.h>
#include <vector>
#include <moonlanderprogram.hpp>
#include <camera.hpp>
#include "constants.hpp"
#include "utils/utils.hpp"

using glm::vec2;

class Level
{
public:
    explicit Level();
    ~Level();
    void extendToRight();
    void extendToLeft();

    GLfloat height_min;
    GLfloat height_max;
    std::vector<vec2> platforms;
    std::vector<vec2> points;
    std::vector<vec2> stars;
};


#endif //MOONLANDER_LEVEL_HPP

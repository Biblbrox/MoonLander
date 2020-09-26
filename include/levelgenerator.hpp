#ifndef MOONLANDER_LEVELGENERATOR_HPP
#define MOONLANDER_LEVELGENERATOR_HPP

#include <SDL_render.h>
#include <vector>
#include <moonlanderprogram.hpp>
#include <camera.hpp>
#include "constants.hpp"
#include "utils/utils.hpp"

using glm::vec2;

class LevelGenerator
{
public:
    explicit LevelGenerator();
    ~LevelGenerator();
    void extendToRight(std::vector<vec2>& points,std::vector<vec2>& stars);
    void extendToLeft(std::vector<vec2>& points, std::vector<vec2>& stars);
    std::vector<vec2> generate_lines(vec2 initial_x);
    std::vector<vec2> generate_stars(GLfloat left, GLfloat right) const;

    GLfloat height_min;
    GLfloat height_max;
    std::vector<vec2> platforms;
private:
    const GLfloat frame_width = 2000;
};


#endif //MOONLANDER_LEVELGENERATOR_HPP

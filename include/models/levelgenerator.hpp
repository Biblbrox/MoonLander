#ifndef MOONLANDER_LEVELGENERATOR_HPP
#define MOONLANDER_LEVELGENERATOR_HPP

#include <SDL_render.h>
#include <vector>
#include <moonlanderprogram.hpp>
#include <camera.hpp>
#include "constants.hpp"
#include "utils.hpp"

using glm::vec2;

class LevelGenerator {
public:
    explicit LevelGenerator();
    ~LevelGenerator();
    void extendToRight(std::vector<vec2>& points,
                       std::vector<vec2>& stars) const;
    void extendToLeft(std::vector<vec2>& points,
                      std::vector<vec2>& stars) const;
    std::vector<vec2> generate_lines(int initial_x) const;
    std::vector<vec2> generate_stars(GLfloat left, GLfloat right) const;

private:
    const GLfloat frame_width = 2000;

    GLfloat height_min;
    GLfloat height_max;
};


#endif //MOONLANDER_LEVELGENERATOR_HPP

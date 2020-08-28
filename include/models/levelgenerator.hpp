#ifndef MOONLANDER_LEVELGENERATOR_HPP
#define MOONLANDER_LEVELGENERATOR_HPP

#include <SDL_render.h>
#include <vector>
#include <moonlanderprogram.hpp>
#include <camera.hpp>
#include "constants.hpp"
#include "utils.hpp"

class LevelGenerator {
public:
    explicit LevelGenerator();
    ~LevelGenerator();
    void extendToRight(std::vector<utils::Point>& points, std::vector<utils::Point>& stars);
    void extendToLeft(std::vector<utils::Point>& points, std::vector<utils::Point>& stars);
    std::vector<utils::Point> generate_lines(int initial_x) const;
    std::vector<utils::Point> generate_stars(GLfloat left, GLfloat right) const;

private:
    int points_count;
    int stars_count;
    const GLfloat frame_width = 2000;

    GLfloat height_min;
    GLfloat height_max;
};


#endif //MOONLANDER_LEVELGENERATOR_HPP

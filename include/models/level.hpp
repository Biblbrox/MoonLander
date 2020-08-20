#ifndef MOONLANDER_LEVEL_HPP
#define MOONLANDER_LEVEL_HPP

#include <SDL_render.h>
#include <vector>
#include <moonlanderprogram.hpp>
#include <camera.hpp>
#include "types.hpp"

class Level {
public:
    explicit Level();
    ~Level();
    bool hasCollision(Utils::Rect coord, GLfloat angle);
    void extendToRight();
    void extendToLeft();

public:
    SurfaceType surfaceType;
    std::vector<Utils::Point> points;
    std::vector<Utils::Point> stars;

    int points_count;
    int stars_count;
    const int frame_width = 2000;

    std::vector<Utils::Point> generate_lines(int initial_x);
    Utils::RandomUniform urand;
    int height_min;
    int height_max;
};


#endif //MOONLANDER_LEVEL_HPP

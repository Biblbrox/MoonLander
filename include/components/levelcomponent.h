#ifndef MOONLANDER_LEVELCOMPONENT_H
#define MOONLANDER_LEVELCOMPONENT_H

#include "../ces/component.hpp"
#include "../models/levelgenerator.hpp"

struct LevelComponent : Component
{
    //LevelGenerator level;
    std::vector<Utils::Point> points;
    std::vector<Utils::Point> stars;

    int points_count;
    int stars_count;
    const int frame_width = 2000;

    int height_min;
    int height_max;
};

#endif //MOONLANDER_LEVELCOMPONENT_H

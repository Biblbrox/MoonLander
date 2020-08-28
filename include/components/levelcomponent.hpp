#ifndef MOONLANDER_LEVELCOMPONENT_HPP
#define MOONLANDER_LEVELCOMPONENT_HPP

#include "../ces/component.hpp"
#include "../models/levelgenerator.hpp"

struct LevelComponent : Component
{
    std::vector<utils::Point> points;
    std::vector<utils::Point> stars;

    GLfloat scale_factor = 1.f;
};

#endif //MOONLANDER_LEVELCOMPONENT_HPP

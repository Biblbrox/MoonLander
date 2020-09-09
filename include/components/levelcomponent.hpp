#ifndef MOONLANDER_LEVELCOMPONENT_HPP
#define MOONLANDER_LEVELCOMPONENT_HPP

#include "../ces/component.hpp"
#include "../models/levelgenerator.hpp"

using glm::vec2;

struct LevelComponent : Component
{
    std::vector<vec2> points;
    std::vector<vec2> stars;

    GLfloat scale_factor = 1.f;
};

#endif //MOONLANDER_LEVELCOMPONENT_HPP

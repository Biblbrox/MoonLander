#ifndef MOONLANDER_COLLISIONCOMPONENT_HPP
#define MOONLANDER_COLLISIONCOMPONENT_HPP

#include "ecs/component.hpp"

struct CollisionComponent : ecs::Component
{
    bool has_collision = false;
};

#endif //MOONLANDER_COLLISIONCOMPONENT_HPP

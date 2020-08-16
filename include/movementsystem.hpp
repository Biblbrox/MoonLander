#ifndef MOONLANDER_MOVEMENTSYSTEM_HPP
#define MOONLANDER_MOVEMENTSYSTEM_HPP

#include "ces/system.hpp"
#include "directioncomponent.hpp"
#include "positioncomponent.hpp"

/**
 * System that can handle game objects with
 * Position and Direction components, like spaceship
 */
class MovementSystem: public System<system_holder<PositionComponent, DirectionComponent>>
{
public:
    void update();
    void render();
};

#endif //MOONLANDER_MOVEMENTSYSTEM_HPP

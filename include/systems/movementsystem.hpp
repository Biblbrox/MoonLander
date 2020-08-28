#ifndef MOONLANDER_MOVEMENTSYSTEM_HPP
#define MOONLANDER_MOVEMENTSYSTEM_HPP

#include <GL/glew.h>
#include <components/velocitycomponent.hpp>
#include "ces/system.hpp"
#include "components/positioncomponent.hpp"

/**
 * System that can handle game objects with
 * Position and Direction components, like spaceship
 */
//class MovementSystem: public System<system_holder<PositionComponent, VelocityComponent>>
class MovementSystem: public System<PositionComponent, VelocityComponent>
{
public:
    explicit MovementSystem();

    void update(size_t delta) override;
};

#endif //MOONLANDER_MOVEMENTSYSTEM_HPP

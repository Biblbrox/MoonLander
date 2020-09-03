#ifndef MOONLANDER_PHYSICS_SYSTEM_HPP
#define MOONLANDER_PHYSICS_SYSTEM_HPP

#include <components/velocitycomponent.hpp>
#include <components/particlespritecomponent.h>
#include "../ces/system.hpp"

class PhysicsSystem : public System <VelocityComponent, ParticleSpriteComponent> {
public:
    void update(size_t delta) override;
};

#endif //MOONLANDER_PHYSICS_SYSTEM_HPP

#ifndef MOONLANDER_PHYSICSSYSTEM_HPP
#define MOONLANDER_PHYSICSSYSTEM_HPP

#include "components/velocitycomponent.hpp"
#include "components/particlespritecomponent.hpp"
#include "ecs/system.hpp"

class PhysicsSystem : public ecs::System <VelocityComponent, ParticleSpriteComponent>
{
public:
    void update_state(size_t delta) override;
};

#endif //MOONLANDER_PHYSICSSYSTEM_HPP

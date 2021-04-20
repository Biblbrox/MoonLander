#ifndef MOONLANDER_PARTICLERENDERSYSTEM_HPP
#define MOONLANDER_PARTICLERENDERSYSTEM_HPP

#include "ecs/system.hpp"
#include "components/particlespritecomponent.hpp"

class ParticleRenderSystem : public ecs::System<ParticleSpriteComponent>
{
public:
    void update_state(size_t delta) override;
};

#endif //MOONLANDER_PARTICLERENDERSYSTEM_HPP

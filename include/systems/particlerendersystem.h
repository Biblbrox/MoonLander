#ifndef MOONLANDER_PARTICLERENDERSYSTEM_H
#define MOONLANDER_PARTICLERENDERSYSTEM_H

#include <ces/system.hpp>
#include <components/particlespritecomponent.h>

class ParticleRenderSystem : public System<ParticleSpriteComponent> {
public:
    void update(size_t delta) override;
};

#endif //MOONLANDER_PARTICLERENDERSYSTEM_H

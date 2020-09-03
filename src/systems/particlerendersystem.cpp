#include <systems/particlerendersystem.h>
#include <cstdlib>
#include <renderer.hpp>

void ParticleRenderSystem::update(size_t delta)
{
    auto program = MoonLanderProgram::getInstance();
    program->setTextureRendering(true);

    auto particles = getEntitiesByTag<ParticleSpriteComponent>();
    for (auto& [key, particle]: particles) {
        auto particleComp = particle->getComponent<ParticleSpriteComponent>();
        for (size_t i = 0; i < particleComp->sprite->getSpritesCount(); ++i) {
            particleComp->sprite->setIdx(i);
            Renderer::render(program,
                             *particleComp->sprite, particleComp->coords[i].x,
                             particleComp->coords[i].y, particleComp->coords[i].angle,
                             1.5f);
        }
    }
}
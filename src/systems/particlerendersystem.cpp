#include <systems/particlerendersystem.hpp>
#include <cstdlib>
#include <render.hpp>

void ParticleRenderSystem::update(size_t delta)
{
    auto program = MoonLanderProgram::getInstance();
    auto particles = getEntitiesByTag<ParticleSpriteComponent>();
    for (const auto& [key, particle]: particles) {
        auto particleComp = particle->getComponent<ParticleSpriteComponent>();
        auto sprite = particleComp->sprite;
        auto coords = particleComp->coords;
        for (size_t i = 0; i < sprite->getSpritesCount(); ++i) {
            sprite->setIdx(i);
            render::drawSprite(program, *sprite, coords[i].x,
                               coords[i].y, coords[i].angle, 1.5f);
        }
    }
}

#ifndef MOONLANDER_PARTICLEENGINE_H
#define MOONLANDER_PARTICLEENGINE_H

#include <sprite.hpp>
#include <ces/entity.hpp>
#include <components/spritecomponent.hpp>
#include <components/velocitycomponent.hpp>
#include <components/displaycomponent.hpp>
#include <components/particlecomponent.h>

class ParticleEngine {
    /**
     * Return clips.size() number generated particles from specific texture
     * Each particle is Entity with SpriteComponent, ParticleComponent,
     * PositionComponent, VelocityComponent and DisplayComponent components.
     * Each particle named by convention "Particle {number of particle}"
     * @param texture_file
     * @param clips
     * @return
     */
    static std::unordered_map<std::string, std::shared_ptr<Entity>>
    generateParticlesFromTexture(const std::string& texture_file,
                                 const std::vector<utils::Rect>& clips)
    {
        using boost::format;

        std::unordered_map<std::string, std::shared_ptr<Entity>> particles;
        particles.reserve(clips.size());

        size_t i = 0;
        for (auto& clip: clips) {
            auto en = std::make_shared<Entity>();
            en->addComponent<SpriteComponent>();
            en->addComponent<ParticleComponent>();
            en->addComponent<PositionComponent>();
            en->addComponent<VelocityComponent>();
            en->addComponent<DisplayComponent>();

            auto sprite = en->getComponent<SpriteComponent>();
            sprite->sprite = std::make_shared<Sprite>(
                    utils::getResourcePath("lunar_lander_bw.png"));
            sprite->sprite->addClipSprite(clip);
            sprite->sprite->generateDataBuffer();
            particles.insert({(format("Particle %1%") % i++).str(), en});
        }
    }


private:
};

#endif //MOONLANDER_PARTICLEENGINE_H

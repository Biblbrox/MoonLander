#ifndef MOONLANDER_PARTICLEENGINE_HPP
#define MOONLANDER_PARTICLEENGINE_HPP

#include <sprite.hpp>
#include <ces/entity.hpp>
#include <components/spritecomponent.hpp>
#include <components/velocitycomponent.hpp>
#include <components/particlespritecomponent.hpp>

class ParticleEngine
{
public:
    /**
     * Return clips.size() number generated particles from specific texture
     * Each particle is Entity with SpriteComponent, ParticleComponent,
     * PositionComponent and VelocityComponent
     * Each particle named by convention "Particle {number of particle}"
     * @param texture_file
     * @param clips
     * @return
     */
    static std::shared_ptr<Entity>
    generateFromTexture(const std::string& texture_file,
                                const std::vector<utils::Rect>& clips,
                                const std::vector<utils::Position>& init_coords,
                                const std::vector<utils::Position>& init_vel,
                                GLfloat life_time)
    {
        assert(clips.size() == init_coords.size()
               && "Number of particles must be "
                  "the same as number of coordinates");

        assert(init_coords.size() == init_vel.size()
               && "Number of coordinates must be "
                  "the same as number of velocities");

        auto en = std::make_shared<Entity>();
        en->addComponent<ParticleSpriteComponent>();
        en->addComponent<VelocityComponent>();
        en->activate();

        auto particle = en->getComponent<ParticleSpriteComponent>();
        particle->sprite = std::make_shared<Sprite>(texture_file);
        particle->is_alive = true;
        particle->life_time = life_time;

        for (size_t i = 0; i < clips.size(); ++i) {
            particle->sprite->addClipSprite(clips[i]);
            particle->coords.push_back({init_coords[i].x,
                                        init_coords[i].y,
                                        init_coords[i].angle});
            particle->vel.push_back({init_vel[i].x,
                                     init_vel[i].y,
                                     init_vel[i].angle});
        }

        particle->sprite->generateDataBuffer();

        return en;
    }

private:
};

#endif //MOONLANDER_PARTICLEENGINE_HPP

#ifndef MOONLANDER_WORLD_HPP
#define MOONLANDER_WORLD_HPP

#include <unordered_map>
#include <memory>
#include <string>
#include <utils/fps.hpp>
#include "utils/utils.hpp"
#include "basesystem.hpp"
#include "camera.hpp"
#include "audio.hpp"

/**
 * To avoid circular including
 */
class Entity;
class Component;

using utils::type_id;

class World
{
public:
    World() = default;
    ~World() = default;

    void init();
    void update(size_t delta);

    Entity& createEntity(const std::string& name);

    template <typename SystemType>
    SystemType& createSystem()
    {
        static_assert(std::is_base_of_v<BaseSystem, SystemType>,
                      "Template parameter class must be child of BaseSystem");

        std::shared_ptr<SystemType> system(new SystemType());
        system->setWorld(std::shared_ptr<World>(this));
        m_systems.insert({type_id<SystemType>(),
                        std::static_pointer_cast<BaseSystem>(system)});
        return *system;
    }

    std::unordered_map<std::string, std::shared_ptr<Entity>> & getEntities();

private:
    std::unordered_map<std::string, std::shared_ptr<Entity>> m_entities;
    std::unordered_map<size_t, std::shared_ptr<BaseSystem>> m_systems;
    std::vector<std::shared_ptr<Entity>> m_nonStatic;
    Camera m_camera;
    GLuint m_screenHeight;
    GLuint m_screenWidth;
    GLfloat m_frameHeight;
    GLfloat m_frameWidth;

    utils::Fps m_fps;

    void move_from_camera();

    void update_ship();
    void update_text();
    void rescale_world();
    void init_sound();
    void init_sprites();
    void init_text();
    void init_level();

    /**
     * Remove all entities that not alive
     */
    void filter_entities();

    bool m_scaled = false;
    const GLfloat m_scaleFactor = 1.5f;
    utils::audio::Audio m_audio;
};

#endif //MOONLANDER_WORLD_HPP

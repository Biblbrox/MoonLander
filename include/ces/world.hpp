#ifndef MOONLANDER_WORLD_HPP
#define MOONLANDER_WORLD_HPP

#include <unordered_map>
#include <memory>
#include <string>
#include <fps.h>
#include "../utils.hpp"
#include "basesystem.h"
#include "camera.hpp"
#include "entity.hpp"

/**
 * To avoid circular including
 */
//class Entity;
class Component;

using Utils::type_id;

class World {
public:
    World() = default;
    ~World() = default;

    void init();
    void update(size_t delta);

    Entity& createEntity(std::string name);

    template <typename SystemType>
    SystemType& createSystem()
    {
        std::shared_ptr<SystemType> system(new SystemType());
        system->setWorld(std::shared_ptr<World>(this));
        systems.insert({type_id<SystemType>(), std::static_pointer_cast<BaseSystem>(system)});
        return *system;
    }

    std::unordered_map<std::string, std::shared_ptr<Entity>> & getEntities();

private:
    std::unordered_map<std::string, std::shared_ptr<Entity>> entities;
    std::unordered_map<size_t, std::shared_ptr<BaseSystem>> systems;
    std::vector<Entity> non_static;
    Camera camera;
    GLuint screen_height;
    GLuint screen_width;
    GLfloat frame_height;
    GLfloat frame_width;

    Fps fps;

    void move_from_camera();

    void update_ship();
    void update_text();

    bool scaled = false;
    const GLfloat scale_factor = 1.5f;
};

#endif //MOONLANDER_WORLD_HPP

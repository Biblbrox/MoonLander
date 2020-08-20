#ifndef MOONLANDER_WORLD_HPP
#define MOONLANDER_WORLD_HPP

#include <unordered_map>
#include <memory>
#include "../utils.hpp"
#include "basesystem.h"
#include "entity.hpp"

/**
 * To avoid circular including
 */
class Entity;
class Component;

using Utils::type_id;

class World {
public:
    World() = default;
    ~World() = default;

    void init();
    void update(size_t delta);

    Entity& createEntity();

    template <typename SystemType>
    SystemType& createSystem()
    {
        std::shared_ptr<SystemType> system(new SystemType());
        system->setWorld(std::shared_ptr<World>(this));
        systems.insert({type_id<SystemType>(), std::static_pointer_cast<BaseSystem>(system)});
        return dynamic_cast<SystemType&>(*system);
    }

    std::vector<Entity>& getEntities();

private:
    std::vector<Entity> entities;
    std::unordered_map<size_t, std::shared_ptr<BaseSystem>> systems;
};

#endif //MOONLANDER_WORLD_HPP

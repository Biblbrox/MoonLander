#ifndef MOONLANDER_WORLD_HPP
#define MOONLANDER_WORLD_HPP

#include <unordered_map>
#include <memory>
#include "../utils.hpp"

/**
 * To avoid circular including
 */
class Entity;
class BaseSystem;
class Component;

class World {
public:
    World() = default;
    ~World() = default;

    Entity createEntity();

    template <typename SystemType>
    void addSystem(BaseSystem& system)
    {
        system_map.insert({Utils::type_id<SystemType>(), std::make_unique<SystemType>(system)});
    }

    const std::vector<Entity>& getEntities() const;

private:
    std::vector<Entity> entities;
    std::unordered_map<size_t, std::unique_ptr<BaseSystem>> system_map;
};

#endif //MOONLANDER_WORLD_HPP

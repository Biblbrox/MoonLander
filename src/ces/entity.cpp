#include "../include/ces/entity.hpp"
#include <memory>
#include <ces/component.hpp>

void Entity::activate()
{

}

const std::unordered_map<size_t, std::shared_ptr<Component>>& Entity::getComponents() const
{
    return components;
}

void Entity::setWorld(std::shared_ptr<World> world)
{
    pWorld = world;
}
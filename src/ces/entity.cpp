#include "../include/ces/entity.hpp"
#include <memory>

void Entity::activate()
{

}

void Entity::setWorld(std::shared_ptr<World> world)
{
    pWorld = world;
}


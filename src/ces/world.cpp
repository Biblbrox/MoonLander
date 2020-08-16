#include "../include/ces/world.hpp"
#include "../include/ces/entity.hpp"

Entity World::createEntity()
{
    Entity ent;
    entities.push_back(ent);
    return ent;
}

const std::vector<Entity> &World::getEntities() const
{
    return entities;
}
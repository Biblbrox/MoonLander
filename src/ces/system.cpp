#include "../../include/ces/system.hpp"

#include "../../include/ces/world.hpp"

template <typename... Args>
System<system_holder<Args...>>::System()
{
    fill_types<Args...>();
}

template<typename... Args>
const std::vector<Entity>& System<system_holder<Args...>>::getEntities() const
{
    return pWorld->getEntities();
}

template<typename... Args>
void System<system_holder<Args...>>::setWorld(std::shared_ptr<World> world)
{
    pWorld = world;
}
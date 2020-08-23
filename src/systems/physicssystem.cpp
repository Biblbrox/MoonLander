#include "../../include/systems/physics_system.h"

void PhysicsSystem::update(size_t delta)
{
    auto entities = getEntities();
    for (auto& [key, en] : entities) {
        en->getComponent<VelocityComponent>()->y += gravity_force / weight;
    }
}

#include "../../include/systems/physics_system.hpp"

void PhysicsSystem::update(size_t delta)
{
    auto entities = getEntitiesByTag<VelocityComponent>();
    for (auto& [key, en] : entities)
        en->getComponent<VelocityComponent>()->y += gravity_force / weight;

    auto particles = getEntitiesByTag<ParticleSpriteComponent>();
    for (auto& [key, en] : particles) {
        auto coords = en->getComponent<ParticleSpriteComponent>()->coords;
        for (auto& coord: coords)
            coord.y += gravity_force / weight;
    }
}

#include "../../include/systems/physics_system.h"

const GLfloat gravity_force = 0.5f;
const GLfloat weight = 40.f;

void PhysicsSystem::update(size_t delta)
{
    auto entities = getEntities();
    for (auto& [key, en] : entities) {
        en->getComponent<VelocityComponent>()->y += gravity_force / weight;
    }
}

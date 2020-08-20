#include "systems/movementsystem.hpp"

const GLfloat gravity_force = 0.5f;
const GLfloat weight = 40.f;
const GLfloat engine_force = 1.f;
const GLfloat rot_step = 0.004f;

void MovementSystem::update(size_t delta)
{
    std::vector<Entity> entities = getEntities();
    for (auto& en: entities) {
        auto pos = en.getComponent<PositionComponent>();
        auto vel = en.getComponent<VelocityComponent>();

        pos->x += vel->x;
        pos->y += vel->y;
        pos->angle += vel->vel_angle;

        vel->y += gravity_force / weight;
    }
}

MovementSystem::MovementSystem()
{

}

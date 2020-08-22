#include "systems/movementsystem.hpp"

void MovementSystem::update(size_t delta)
{
    auto entities = getEntities();
    for (auto& [key, en]: entities) {
        auto pos = en->getComponent<PositionComponent>();
        auto vel = en->getComponent<VelocityComponent>();

        pos->x += vel->x;
        pos->y += vel->y;
        pos->angle += vel->vel_angle;
    }
}

MovementSystem::MovementSystem()
{

}

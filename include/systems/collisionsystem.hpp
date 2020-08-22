#ifndef MOONLANDER_COLLISIONSYSTEM_HPP
#define MOONLANDER_COLLISIONSYSTEM_HPP

#include <components/collisioncomponent.hpp>
#include <components/spritecomponent.hpp>
#include <components/levelcomponent.h>
#include <components/positioncomponent.hpp>
#include "../ces/system.hpp"

class CollisionSystem : public System<system_holder<CollisionComponent, SpriteComponent, LevelComponent, PositionComponent>> {
    void update(size_t delta) override;

private:
    bool levelSpriteCollision(Sprite &sprite, GLfloat ship_x,
                                               GLfloat ship_y, std::vector<Utils::Point>& points,
                                               std::vector<Utils::Point>& stars, GLfloat angle);
};

#endif //MOONLANDER_COLLISIONSYSTEM_HPP

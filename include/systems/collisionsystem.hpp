#ifndef MOONLANDER_COLLISIONSYSTEM_HPP
#define MOONLANDER_COLLISIONSYSTEM_HPP

#include <components/collisioncomponent.hpp>
#include <components/spritecomponent.hpp>
#include <components/levelcomponent.hpp>
#include <components/positioncomponent.hpp>
#include "../ces/system.hpp"

//class CollisionSystem : public System<system_holder<CollisionComponent, SpriteComponent, LevelComponent, PositionComponent>> {
class CollisionSystem : public System<CollisionComponent, SpriteComponent, LevelComponent, PositionComponent> {
    void update(size_t delta) override;

private:
    bool levelSpriteCollision(Sprite &sprite, GLfloat ship_x,
                                               GLfloat ship_y, std::vector<utils::Point>& points,
                                               std::vector<utils::Point>& stars, GLfloat angle);
};

#endif //MOONLANDER_COLLISIONSYSTEM_HPP

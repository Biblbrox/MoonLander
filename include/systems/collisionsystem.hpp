#ifndef MOONLANDER_COLLISIONSYSTEM_HPP
#define MOONLANDER_COLLISIONSYSTEM_HPP

#include <components/collisioncomponent.hpp>
#include <components/spritecomponent.hpp>
#include <components/levelcomponent.hpp>
#include <components/positioncomponent.hpp>
#include "../ces/system.hpp"

using glm::vec2;

class CollisionSystem : public System<CollisionComponent, SpriteComponent, LevelComponent, PositionComponent> {
    void update(size_t delta) override;

private:
    bool levelSpriteCollision(Sprite &sprite, GLfloat ship_x,
                                               GLfloat ship_y, std::vector<vec2>& points,
                                               std::vector<vec2>& stars, GLfloat angle);
};

#endif //MOONLANDER_COLLISIONSYSTEM_HPP

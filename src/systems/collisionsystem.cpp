#include <systems/collisionsystem.hpp>

using glm::vec2;

void CollisionSystem::update(size_t delta)
{
    auto levels = getEntitiesByTag<LevelComponent>();
    auto sprites = getEntitiesByTag<SpriteComponent>();

    for (auto it = sprites.begin(); it != sprites.end(); ++it) {
        for (auto jt = sprites.begin(); jt != sprites.end(); ++jt) {
            if (jt != it && it->second->getComponent<CollisionComponent>() != nullptr
                && jt->second->getComponent<CollisionComponent>() != nullptr) {
                // Code to check collision between sprites
            }
        }
    }

    // We need to check we have only one level (otherwise will be strange)
    assert(levels.size() == 1);

    auto level = levels.begin()->second->getComponent<LevelComponent>();
    auto level_col = levels.begin()->second->getComponent<CollisionComponent>();
    for (auto & [key, spriteEntity] : sprites) {
        if (spriteEntity->getComponent<CollisionComponent>() != nullptr) {
            auto sprite = spriteEntity->getComponent<SpriteComponent>()->sprite;
            auto pos = spriteEntity->getComponent<PositionComponent>();
            if (levelSpriteCollision(*sprite, pos->x, pos->y,
                                     level->points,
                                     level->stars, pos->angle)) {
                spriteEntity->getComponent<CollisionComponent>()
                        ->has_collision = true;
                level_col->has_collision = true;
            } else {
                spriteEntity->getComponent<CollisionComponent>()->has_collision = false;
                levels.begin()->second->
                        getComponent<CollisionComponent>()->has_collision = false;
            }
        }
    }

}

bool CollisionSystem::levelSpriteCollision(Sprite &sprite, GLfloat ship_x,
                                           GLfloat ship_y, std::vector<vec2>& points,
                                           std::vector<vec2>& stars, GLfloat angle) {
    utils::Rect coord = sprite.getCurrentClip();
    coord.x = ship_x;
    coord.y = ship_y;
    utils::RectPoints r = utils::collision::buildRectPoints(coord, angle);

    using utils::collision::lineLine;
    for (size_t i = 0; i < points.size() - 1; ++i) {
        GLfloat x = std::min({r.a.x, r.b.x, r.c.x, r.d.x});
        GLfloat curX = points[i].x;
        GLfloat curY = points[i].y;
        GLfloat nextX = points[i + 1].x;
        GLfloat nextY = points[i + 1].y;

        if (points[i].x <= x && points[i + 1].x >= x) {
            bool left = lineLine(r.d, r.a, {curX, curY},{nextX, nextY});
            bool right = lineLine(r.b, r.c, {curX, curY},
                                         {points[i + 1].x, points[i + 1].y});
            bool top = lineLine(r.c, r.d, {points[i].x, points[i].y},
                                       {points[i + 1].x, points[i + 1].y});
            bool bottom = lineLine(r.a, r.b, {points[i].x, points[i].y},
                                          {points[i + 1].x, points[i + 1].y});
            if (left || right || top || bottom)
                return true;

            x = std::max({r.a.x, r.b.x, r.c.x, r.d.x});
            if (points[i + 1].x <= x && i != points.size() - 2) {
                left = lineLine(r.d, r.a, {points[i + 1].x, points[i + 1].y},
                                       {points[i + 2].x, points[i + 2].y});
                right = lineLine(r.b, r.c, {points[i + 1].x, points[i + 1].y},
                                        {points[i + 2].x, points[i + 2].y});
                top = lineLine(r.c, r.d, {points[i + 1].x, points[i + 1].y},
                                      {points[i + 2].x, points[i + 2].y});
                bottom = lineLine(r.a, r.b, {points[i + 1].x, points[i + 1].y},
                                         {points[i + 2].x, points[i + 2].y});
                if (left || right || top || bottom)
                    return true;
            }
        }
    }

    return false;
}

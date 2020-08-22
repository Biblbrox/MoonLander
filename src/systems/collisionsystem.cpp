#include <systems/collisionsystem.hpp>

void CollisionSystem::update(size_t delta)
{
    auto levelEntities = getEntitiesByTag<LevelComponent>();
    auto spriteEntities = getEntitiesByTag<SpriteComponent>();

    for (auto it = spriteEntities.begin(); it != spriteEntities.end(); ++it) {
        for (auto jt = spriteEntities.begin(); jt != spriteEntities.end(); ++jt) {
            if (jt != it) {
                // Code to check collision between sprites
            }
        }
    }

    // We need to check we have only one level (otherwise will be strange)
    assert(levelEntities.size() == 1);

    auto level = levelEntities.begin()->second->getComponent<LevelComponent>();
    auto level_col = levelEntities.begin()->second->getComponent<CollisionComponent>();
    for (auto & [key, spriteEntity] : spriteEntities) {
        auto sprite = spriteEntity->getComponent<SpriteComponent>()->sprite;
        auto pos = spriteEntity->getComponent<PositionComponent>();
        if (levelSpriteCollision(*sprite, pos->x, pos->y, level->points, level->stars, pos->angle)) {
            spriteEntity->getComponent<CollisionComponent>()->has_collision = true;
            level_col->has_collision = true;
        } else {
            spriteEntity->getComponent<CollisionComponent>()->has_collision = false;
            levelEntities.begin()->second->getComponent<CollisionComponent>()->has_collision = false;
        }
    }

}

bool CollisionSystem::levelSpriteCollision(Sprite &sprite, GLfloat ship_x,
                                           GLfloat ship_y, std::vector<Utils::Point>& points,
                                           std::vector<Utils::Point>& stars, GLfloat angle) {
    Utils::Rect coord = sprite.getCurrentClip();
    coord.x = ship_x;
    coord.y = ship_y;
    Utils::RectPoints r = Utils::buildRectPoints(coord, angle);

    for (size_t i = 0; i < points.size() - 1; ++i) {
        GLfloat x = std::min({r.a.x, r.b.x, r.c.x, r.d.x});

        if (points[i].x <= x && points[i + 1].x >= x) {
            bool left = Utils::lineLine(r.d, r.a, {.x = points[i].x, .y = points[i].y},
                                        {.x = points[i + 1].x, .y = points[i + 1].y});
            bool right = Utils::lineLine(r.b, r.c, {.x = points[i].x, .y = points[i].y},
                                         {.x = points[i + 1].x, .y = points[i + 1].y});
            bool top = Utils::lineLine(r.c, r.d, {.x = points[i].x, .y = points[i].y},
                                       {.x = points[i + 1].x, .y = points[i + 1].y});
            bool bottom = Utils::lineLine(r.a, r.b, {.x = points[i].x, .y = points[i].y},
                                          {.x = points[i + 1].x, .y = points[i + 1].y});
            if (left || right || top || bottom)
                return true;

            x = std::max({r.a.x, r.b.x, r.c.x, r.d.x});
            if (points[i + 1].x <= x && i != points.size() - 2) {
                left = Utils::lineLine(r.d, r.a, {.x = points[i + 1].x, .y = points[i + 1].y},
                                       {.x = points[i + 2].x, .y = points[i + 2].y});
                right = Utils::lineLine(r.b, r.c, {.x = points[i + 1].x, .y = points[i + 1].y},
                                        {.x = points[i + 2].x, .y = points[i + 2].y});
                top = Utils::lineLine(r.c, r.d, {.x = points[i + 1].x, .y = points[i + 1].y},
                                      {.x = points[i + 2].x, .y = points[i + 2].y});
                bottom = Utils::lineLine(r.a, r.b, {.x = points[i + 1].x, .y = points[i + 1].y},
                                         {.x = points[i + 2].x, .y = points[i + 2].y});
                if (left || right || top || bottom)
                    return true;
            }
        }
    }

    return false;
}

#include <systems/collisionsystem.hpp>
#include "../include/utils/utils.hpp"

using glm::vec2;

/**
 * Check whether first line (p11, p12) intersect with second (p21, p22)
 * @param p11
 * @param p12
 * @param p21
 * @param p22
 * @return
*/
constexpr bool lineLine(const vec2 &p11, const vec2 &p12, const vec2 &p21,
                        const vec2 &p22) noexcept
{
    GLfloat x1 = p11.x;
    GLfloat x2 = p12.x;
    GLfloat x3 = p21.x;
    GLfloat x4 = p22.x;

    GLfloat y1 = p11.y;
    GLfloat y2 = p12.y;
    GLfloat y3 = p21.y;
    GLfloat y4 = p22.y;

    // calculate the distance to intersection point
    GLfloat uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3))
                 / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
    GLfloat uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3))
                 / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

    // if uA and uB are between 0-1, lines are colliding
    return uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1;
}

inline utils::RectPoints
buildRectPoints(const utils::Rect &rect, double rot) noexcept
{
    utils::RectPoints temp_rect{};
    GLfloat bx, by, cx, cy, dx, dy;
    GLfloat x = rect.x;
    GLfloat y = rect.y;
    GLfloat wid = rect.w;
    GLfloat hgt = rect.h;

    bx = x + wid * cos(rot);
    by = y + wid * sin(rot);

    dx = x - hgt * sin(rot);
    dy = y + hgt * cos(rot);

    cx = bx - hgt * sin(rot);
    cy = dy + wid * sin(rot);

    temp_rect.a.x = x;
    temp_rect.a.y = y;
    temp_rect.b.x = bx;
    temp_rect.b.y = by;
    temp_rect.c.x = cx;
    temp_rect.c.y = cy;
    temp_rect.d.x = dx;
    temp_rect.d.y = dy;

    return temp_rect;
}

void CollisionSystem::update_state(size_t delta)
{
    auto levels = getEntitiesByTag<LevelComponent>();
    auto sprites = getEntitiesByTags<SpriteComponent, CollisionComponent>();

    //for (auto it = sprites.begin(); it != sprites.end(); ++it) {
    //    for (auto jt = sprites.begin(); jt != sprites.end(); ++jt) {
    //        if (jt != it &&
    //            it->second->getComponent<CollisionComponent>() != nullptr
    //            && jt->second->getComponent<CollisionComponent>() != nullptr) {
    //            // Code to check collision between sprites
    //        }
    //    }
    //}

    // We need to check we have only one level (otherwise will be strange)
    assert(levels.size() == 1);

    auto level = levels.begin()->second->getComponent<LevelComponent>();
    auto levelCol = levels.begin()->second->getComponent<CollisionComponent>();
    for (auto &[key, spriteEntity] : sprites) {
        auto colComponent = spriteEntity->getComponent<CollisionComponent>();
        auto sprite = spriteEntity->getComponent<SpriteComponent>()->sprite;
        auto pos = spriteEntity->getComponent<PositionComponent>();
        if (utils::physics::altitude(level->points, pos->x, pos->y) >= 250)
            return;

        if (levelSpriteCollision(*sprite, pos->x, pos->y,
                                 level->points, level->stars, pos->angle)) {
            colComponent->has_collision = true;
            levelCol->has_collision = true;
        } else {
            colComponent->has_collision = false;
            levelCol->has_collision = false;
        }
    }
}

bool
CollisionSystem::levelSpriteCollision(const Sprite &sprite, GLfloat ship_x,
                                      GLfloat ship_y, const std::vector<vec2> &points,
                                      const std::vector<vec2> &stars, GLfloat angle)
{
    utils::Rect coord = sprite.getCurrentClip();
    coord.x = ship_x;
    coord.y = ship_y;
    utils::RectPoints r = buildRectPoints(coord, angle);

    GLfloat x = std::min({r.a.x, r.b.x, r.c.x, r.d.x});
    auto rightBound = std::lower_bound(points.cbegin(), points.cend(), glm::vec2(x, 0.f),
                                       [](const glm::vec2& pos1, const glm::vec2& pos2){
                                           return pos1.x < pos2.x;
    });
    auto leftBound = std::prev(rightBound);
    auto nextRightBound = std::next(rightBound);

    GLfloat curX = leftBound->x;
    GLfloat curY = leftBound->y;
    GLfloat nextX = rightBound->x;
    GLfloat nextY = rightBound->y;
    GLfloat nextNextX = nextRightBound == points.end() ? 0 : rightBound->x;
    GLfloat nextNextY = nextRightBound == points.end() ? 0 : rightBound->y;

    bool left = lineLine(r.d, r.a, {curX, curY}, {nextX, nextY});
    bool right = lineLine(r.b, r.c, {curX, curY}, {nextX, nextY});
    bool top = lineLine(r.c, r.d, {curX, curY}, {nextX, nextY});
    bool bottom = lineLine(r.a, r.b, {curX, curY}, {nextX, nextY});
    if (left || right || top || bottom)
        return true;

    if (rightBound->x <= x && nextRightBound != points.end()) {
        left = lineLine(r.d, r.a, {nextX, nextY}, {nextNextX, nextNextY});
        right = lineLine(r.b, r.c, {nextX, nextY}, {nextNextX, nextNextY});
        top = lineLine(r.c, r.d, {nextX, nextY}, {nextNextX, nextNextY});
        bottom = lineLine(r.a, r.b, {nextX, nextY}, {nextNextX, nextNextY});
        if (left || right || top || bottom)
            return true;
    }

    return false;
}

#include <systems/collisionsystem.hpp>
#include "../../include/utils/collision.hpp"
#include <numeric>
#include <iostream>

using glm::vec2;

void CollisionSystem::update_state(size_t delta)
{
    auto levels = getEntitiesByTag<LevelComponent>();
    auto sprites = getEntitiesByTags<SpriteComponent, CollisionComponent>();

    const int critAlt = 250;

    // We need to check we have only one level (otherwise will be strange)
    assert(levels.size() == 1);

    auto level = levels.begin()->second->getComponent<LevelComponent>();
    auto levelCol = levels.begin()->second->getComponent<CollisionComponent>();
    for (auto &[key, spriteEntity] : sprites) {
        auto colComponent = spriteEntity->getComponent<CollisionComponent>();
        auto sprite = spriteEntity->getComponent<SpriteComponent>()->sprite;
        auto pos = spriteEntity->getComponent<PositionComponent>();
        if (utils::physics::altitude(level->points, pos->x, pos->y) >= critAlt)
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

/**
 * Find lines that affected by any of points of rectangle rect
 * @param rect
 * @param points
 * @return
 */
std::vector<size_t> find_lines_under(const utils::RectPoints& rect,
                                     const std::vector<vec2> &points)
{
    GLfloat left_most = std::min({rect.a.x, rect.b.x, rect.c.x, rect.d.x});
    size_t line_idx = std::prev(
            std::lower_bound(points.cbegin(), points.cend(), left_most,
                             [](const vec2& point, GLfloat val) {
                                 return point.x < val;
                             })) - points.cbegin();

    // Assume that ship can't be place at more than 4 lines simultaneously
    std::vector<size_t> res(4);
    std::iota(res.begin(), res.end(), line_idx);

    return res;
}


bool
CollisionSystem::levelSpriteCollision(const Sprite &sprite, GLfloat ship_x,
                                      GLfloat ship_y, const std::vector<vec2> &points,
                                      const std::vector<vec2> &stars, GLfloat angle)
{
    utils::Rect coords = sprite.getCurrentClip();
    coords.x = ship_x;
    coords.y = ship_y;
    utils::RectPoints r = coll::buildRectPoints(coords, angle);

    std::vector<size_t> lines = find_lines_under(r, points);

    return std::any_of(lines.cbegin(), lines.cend(), [points, r](size_t idx) {
        vec2 p = points[idx];
        vec2 p_right = points[idx + 1];
        bool left = coll::lineLine(r.d, r.a, {p.x, p.y}, {p_right.x, p_right.y});
        bool right = coll::lineLine(r.b, r.c, {p.x, p.y}, {p_right.x, p_right.y});
        bool top = coll::lineLine(r.c, r.d, {p.x, p.y}, {p_right.x, p_right.y});
        bool bottom = coll::lineLine(r.a, r.b, {p.x, p.y}, {p_right.x, p_right.y});

        return left || right || top || bottom;
    });
}
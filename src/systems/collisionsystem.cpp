#include <systems/collisionsystem.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <numeric>
#include <iostream>

using glm::vec2;

/**
 * Check whether first line (p11, p12) intersect with second (p21, p22)
 * @param p11
 * @param p12
 * @param p21
 * @param p22
 * @return
*/
constexpr bool lineLine(const vec2 &p11, const vec2 &p12,
                        const vec2 &p21, const vec2 &p22) noexcept
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

/**
 * Build rectangle from rect with corrected coords by
 * rotation angle
 * @param rect
 * @param alpha
 * @return
 */
inline utils::RectPoints
buildRectPoints(const utils::Rect &rect, GLfloat alpha) noexcept
{
    GLfloat bx, by, cx, cy, dx, dy;
    const GLfloat x = rect.x;
    const GLfloat y = rect.y;
    const GLfloat width = rect.w;
    const GLfloat height = rect.h;
    // angle must be in range 0 - 2*pi
    alpha -= glm::two_pi<GLfloat>()
             * std::floor(alpha / glm::two_pi<GLfloat>());

    alpha = -alpha;

    GLfloat alpha_cos = cos(alpha);
    GLfloat alpha_sin = sin(alpha);

    bx = x + width * alpha_cos;
    by = y - width * alpha_sin;

    dx = x + height * alpha_sin;
    dy = y + height * alpha_cos;

    cx = bx + height * alpha_sin;
    cy = by + height * alpha_cos;

    return {{x, y}, {bx, by}, {cx, cy}, {dx, dy}};
}

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
    utils::RectPoints r = buildRectPoints(coords, angle);

    std::vector<size_t> lines = find_lines_under(r, points);

    return std::any_of(lines.cbegin(), lines.cend(), [points, r](size_t idx) {
        vec2 p = points[idx];
        vec2 p_right = points[idx + 1];
        bool left = lineLine(r.d, r.a, {p.x, p.y}, {p_right.x, p_right.y});
        bool right = lineLine(r.b, r.c, {p.x, p.y}, {p_right.x, p_right.y});
        bool top = lineLine(r.c, r.d, {p.x, p.y}, {p_right.x, p_right.y});
        bool bottom = lineLine(r.a, r.b, {p.x, p.y}, {p_right.x, p_right.y});

        return left || right || top || bottom;
    });
}
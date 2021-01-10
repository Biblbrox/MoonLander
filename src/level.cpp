#include <utils/random.h>
#include <utils/math.h>
#include "level.hpp"

const GLfloat frame_width = 2000;

const int stars_initial_size = 5000;
const int points_initial_size = 500;
const GLfloat deviation = 20.f;

using glm::vec2;
using utils::math::rotate_points;

Level::~Level()
{
}

template <typename BidirectionalIterator>
std::vector<vec2> generate_stars(BidirectionalIterator left,
                                 BidirectionalIterator right, GLfloat gap = 0)
{
    utils::Random rand;
    std::vector<vec2> stars;
    size_t size = std::distance(left, right);
    size_t line_count = size / 2;
    for (auto it = left; it != right - 1; ++it) {
        GLfloat lower = std::max((it + 1)->y, it->y);
        GLfloat higher = std::min((it + 1)->y, it->y);
        for (size_t j = 0; j < stars_initial_size / line_count; ++j)
            stars.emplace_back(rand.generateu(it->x, (it + 1)->x),
                               rand.generateu(lower - gap, higher - gap - frame_width));
    }

    return stars;
}

/**
 * Generate stars with x coordinate in range {left, right}
 * @param left
 * @param right
 * @return
 */
std::vector<vec2> generate_stars(const std::vector<vec2>& points, GLfloat gap = 0)
{
    return generate_stars(points.cbegin(), points.cend(), gap);
}

/**
 * Generate lines where first line has x coordinate as initial_p
 * @param initial_p
 * @return
 */
std::vector<vec2> generate_lines(vec2 initial_p)
{
    //TODO: tune parameters
    const GLfloat point_dist_min = frame_width / points_initial_size * 7.f;
    const GLfloat point_dist_max = frame_width / points_initial_size * 9.f;

    utils::Random rand;

    std::vector<vec2> points;
    points.reserve(points_initial_size);
    points.emplace_back(initial_p.x, rand.generaten(initial_p.y, deviation));
    for (size_t i = 1; i < points_initial_size; ++i)
        points.emplace_back(points[i - 1].x + rand.generateu(point_dist_min, point_dist_max),
                            rand.generaten(points[i - 1].y, deviation)
                            + rand.generateu(-deviation, deviation));

    return points;
}

std::vector<vec2> generate_platforms(std::vector<vec2> &points)
{
    const int plat_count_min = points_initial_size / 8;
    const int plat_count_max = points_initial_size / 3;

    utils::Random rand;

    size_t platforms_count = rand.generateu(plat_count_min, plat_count_max);
    std::vector<GLuint> plat_idx(platforms_count, 0);
    rand.fill_unique(plat_idx.begin(), plat_idx.end(), 0UL, points.size() - 2,
                      true);

    std::vector<vec2> platforms;
    platforms.reserve(platforms_count);
    for (unsigned int i : plat_idx) {
        points[i + 1].y = points[i].y;
        platforms.emplace_back(points[i]);
        platforms.emplace_back(points[i + 1]);
    }
    platforms.shrink_to_fit();

    return platforms;
}

Level::Level() : max_right(0.f), max_left(0.f)
{
    point_dist_min = frame_width / points_initial_size * 7.f;
    point_dist_max = frame_width / points_initial_size * 9.f;
}

void Level::extendToRight(const Camera& camera)
{
    utils::Random rand;
    GLfloat point_x = points.empty()
                      ? 0.f
                      : points.back().x
                        + rand.generateu(point_dist_min, point_dist_max);
    GLfloat point_y = points.empty()
                      ? (height_min + height_max) / 2.f
                      : points.back().y + rand.generateu(-deviation, deviation);
    vec2 begin = vec2(point_x, point_y);
    std::vector<vec2> part_lines = generate_lines(begin);
    std::vector<vec2> part_platforms = generate_platforms(part_lines);
    points.insert(points.end(), part_lines.cbegin(), part_lines.cend());
    platforms.insert(platforms.end(), part_platforms.cbegin(), part_platforms.cend());

    max_left = points[0].x + camera.getX();
    max_right = points.back().x + camera.getX();

    std::vector<vec2> part_stars = generate_stars(part_lines);
    stars.insert(stars.end(), part_stars.cbegin(), part_stars.cend());
}

void Level::extendToLeft(const Camera& camera)
{
    utils::Random rand;
    GLfloat point_x = points.empty()
                      ? -rand.generateu(point_dist_min, point_dist_max)
                      : points[0].x - rand.generateu(point_dist_min, point_dist_max);
    GLfloat point_y = points.empty()
                      ? (height_min + height_max) / 2.f
                      : (points[0].y + rand.generateu(-deviation, deviation));
    vec2 begin = vec2(point_x, point_y);
    std::vector<vec2> part_lines = generate_lines(begin);
    part_lines = rotate_points(part_lines, 0);
    std::vector<vec2> part_platforms = generate_platforms(part_lines);
    points.insert(points.begin(), part_lines.cbegin(), part_lines.cend());
    platforms.insert(platforms.begin(), part_platforms.cbegin(), part_platforms.cend());

    // Update level borders
    max_left = points[0].x + camera.getX();
    max_right = points.back().x + camera.getX();

    std::vector<vec2> part_stars = generate_stars(part_lines);
    stars.insert(stars.begin(), part_stars.cbegin(), part_stars.cend());
}

void Level::extendToUp(std::vector<vec2>::iterator left,
                       std::vector<vec2>::iterator right)
{
    // TODO: add stars extends to up
}

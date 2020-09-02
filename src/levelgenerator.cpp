#include "models/levelgenerator.hpp"
#include "utils.hpp"

const int stars_initial_size = 500;
const int points_initial_size = 500;

using utils::Point;

LevelGenerator::~LevelGenerator()
{

}

std::vector<Point> LevelGenerator::generate_stars(GLfloat left, GLfloat right) const
{
    utils::RandomUniform urand;
    std::vector<Point> gen(stars_initial_size);

    for (auto& star: gen) {
        star.x = urand.generateu(left, right);
        star.y = urand.generateu<GLfloat>(0, height_max);
    }

    return gen;
}

std::vector<Point> LevelGenerator::generate_lines(int initial_x) const
{
    //TODO: tune parameters
    utils::RandomUniform urand;
    std::vector<Point> res(points_count);

    const int platform_count_min = points_count / 8;
    const int platform_count_max = points_count / 3;

    const GLfloat point_distance_min = frame_width / points_count * 4.f;
    const GLfloat point_distance_max = frame_width / points_count * 5.f;

    const GLfloat platform_min_width = 30.f;
    const GLfloat plaftorm_max_width = 50.f;

    size_t platforms_count = urand.generateu(platform_count_min, platform_count_max);
    std::vector<GLuint> plat_idx(platforms_count, 0);

    urand.fill_unique<GLuint>(plat_idx.begin(), plat_idx.end(), 0, res.size());

    GLfloat plat_width;
    const GLfloat deviation = 20.f;
    for (size_t i = 0; i < res.size(); ++i) {
        if (i == 0) {
            res[i].x = initial_x;
            res[i].y = urand.generaten((height_min + height_max) / 2.f, deviation);
        } else {
            if (std::count(plat_idx.begin(), plat_idx.end(), i) != 0) { // platform case
                plat_width = urand.generateu(platform_min_width, plaftorm_max_width);
                res[i].x = res[i - 1].x + urand.generateu(point_distance_min, point_distance_max);
                res[i].y = urand.generaten(res[i - 1].y, deviation)
                           + urand.generateu(-deviation, deviation);
            } else if (std::count(plat_idx.begin(), plat_idx.end(), i - 1) != 0) { // prev. platform case
                res[i].x = res[i - 1].x + plat_width;
                res[i].y = res[i - 1].y;
            } else { // normal case
                res[i].x = res[i - 1].x + urand.generateu(point_distance_min, point_distance_max);
                res[i].y = urand.generaten(res[i - 1].y, deviation)
                           + urand.generateu(-deviation, deviation);
            }
        }
    }

    return res;
}

LevelGenerator::LevelGenerator() : points_count(points_initial_size), stars_count(stars_initial_size)
{
    height_min = utils::getScreenHeight<GLfloat>() - utils::getScreenHeight<GLfloat>() / 2;
    height_max = utils::getScreenHeight<GLfloat>() - utils::getScreenHeight<GLfloat>() / 5;
}

void LevelGenerator::extendToRight(std::vector<Point>& points, std::vector<Point>& stars) const
{
    auto right = generate_lines(points[points.size() - 1].x);

    GLfloat old_right = points[points.size() - 1].x;
    GLfloat line_length = std::abs(right[right.size() - 1].x - points[points.size() - 1].x);

    points.reserve(points.size() + right.size());
    points.insert(points.end(), right.begin(), right.end());

    GLfloat old_max_x = old_right;
    auto new_stars = generate_stars(old_max_x, old_max_x + line_length);
    stars.reserve(stars.size() + new_stars.size());
    stars.insert(stars.end(), new_stars.begin(), new_stars.end());
}

void LevelGenerator::extendToLeft(std::vector<Point>& points, std::vector<Point>& stars) const
{
    auto left = generate_lines(0);

    GLfloat old_left = points[0].x;
    GLfloat line_length = std::abs(points[0].x - left[left.size() - 1].x);

    GLfloat initial_x = -left[left.size() - 1].x;
    std::for_each(left.begin(), left.end(), [initial_x](Point& p){ p.x += initial_x; });

    points.insert(points.end(), left.begin(), left.end());
    std::rotate(points.begin(), points.begin() + left.size(), points.end());

    GLfloat old_max_x = old_left;
    auto new_stars = generate_stars(old_max_x - line_length, old_max_x);
    stars.reserve(stars.size() + new_stars.size());
    stars.insert(stars.end(), new_stars.begin(), new_stars.end());
}
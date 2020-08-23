#include "models/levelgenerator.hpp"
#include "utils.hpp"

const int stars_initial_size = 500;
const int points_initial_size = 100;

using Utils::Point;

LevelGenerator::~LevelGenerator()
{

}

std::vector<Point> LevelGenerator::generate_stars(GLfloat left, GLfloat right) const
{
    Utils::RandomUniform urand;
    std::vector<Point> gen(stars_initial_size);

    for (auto& star: gen) {
        star.x = urand.generate(left, right);
        star.y = urand.generate(0, height_max);
    }

    return gen;
}

std::vector<Point> LevelGenerator::generate_lines(int initial_x) const
{
    Utils::RandomUniform urand;
    std::vector<Point> res(points_count);

    const int platform_count_min = points_count / 8;
    const int platform_count_max = points_count / 3;

    const int point_distance_min = frame_width / points_count * 2;
    const int point_distance_max = std::floor(frame_width / (GLfloat)points_count * 2.5f);

    const int platform_min_width = point_distance_min / 2.f;
    const int plaftorm_max_width = point_distance_min / 1.1f;

    size_t platforms_count = urand.generate(platform_count_min, platform_count_max);
    std::vector<GLuint> plat_idx(platforms_count, 0);

    urand.fill_unique<GLuint>(plat_idx.begin(), plat_idx.end(), 0, res.size());

    GLfloat prev_width;
    for (int i = 0; i < res.size(); ++i) {
        if (i == 0) {
            res[i].x = initial_x;
            res[i].y = urand.generate(height_min, height_max);
        } else {
            if (std::find(plat_idx.begin(), plat_idx.end(), i) != plat_idx.end()) { // platform case
                prev_width = urand.generate(platform_min_width, plaftorm_max_width);
                res[i].x = res[i - 1].x + urand.generate(point_distance_min, point_distance_max) - prev_width / 2.f;
                res[i].y = urand.generate(height_min, height_max);
            } else if (std::find(plat_idx.begin(), plat_idx.end(), i - 1) != plat_idx.end()) { // prev. platform case
                res[i].x = res[i - 1].x + prev_width / 2;
                res[i].y = res[i - 1].y;
            } else { // normal case
                res[i].x = res[i - 1].x + urand.generate(point_distance_min, point_distance_max);
                res[i].y = urand.generate(height_min, height_max);
            }
        }
    }

    return res;
}

LevelGenerator::LevelGenerator() : points_count(points_initial_size), stars_count(stars_initial_size)
{
    height_min = Utils::getScreenHeight<int>() - Utils::getScreenHeight<int>() / 2;
    height_max = Utils::getScreenHeight<int>() - Utils::getScreenHeight<int>() / 5;
}

void LevelGenerator::extendToRight(std::vector<Point>& points, std::vector<Point>& stars)
{
    Utils::RandomUniform urand;
    auto right = generate_lines(points[points.size() - 1].x);

    GLfloat old_right = points[points.size() - 1].x;
    GLfloat line_length = std::abs(right[right.size() - 1].x - points[points.size() - 1].x);

    points.reserve(points.size() + right.size());
    points.insert(points.end(), right.begin(), right.end());

    size_t old_size = stars.size();
    GLfloat old_max_x = old_right;
    stars.resize(stars.size() + stars_initial_size);
    for (size_t i = old_size; i < stars.size(); ++i) {
        stars[i].x = urand.generate(old_max_x, old_max_x + line_length);
        stars[i].y = urand.generate(0, height_max);
    }
}

void LevelGenerator::extendToLeft(std::vector<Point>& points, std::vector<Point>& stars)
{
    Utils::RandomUniform urand;
    auto left = generate_lines(0);

    GLfloat old_left = points[0].x;
    GLfloat line_length = std::abs(points[0].x - left[left.size() - 1].x);

    GLfloat initial_x = -left[left.size() - 1].x;
    std::for_each(left.begin(), left.end(), [initial_x](Point& p){ p.x += initial_x; });

    points.insert(points.end(), left.begin(), left.end());
    std::rotate(points.begin(), points.begin() + left.size(), points.end());

    size_t old_size = stars.size();
    GLfloat old_max_x = old_left;
    stars.resize(stars.size() + stars_initial_size);
    for (size_t i = old_size; i < stars.size(); ++i) {
        stars[i].x = urand.generate<GLfloat>(old_max_x - line_length, old_max_x);
        stars[i].y = urand.generate<GLfloat>(0, height_max);
    }
}
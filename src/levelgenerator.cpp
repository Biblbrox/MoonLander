#include "levelgenerator.hpp"
#include "utils.hpp"

const int stars_initial_size = 500;
const int points_initial_size = 500;

using glm::vec2;

LevelGenerator::~LevelGenerator()
{

}

std::vector<vec2>
LevelGenerator::generate_stars(GLfloat left, GLfloat right) const
{
    utils::RandomUniform urand;
    std::vector<vec2> gen(stars_initial_size);

    for (auto& star: gen) {
        star.x = urand.generateu(left, right);
        star.y = urand.generateu<GLfloat>(0, height_max);
    }

    return gen;
}

std::vector<vec2> LevelGenerator::generate_lines(vec2 initial_p)
{
    //TODO: tune parameters
    utils::RandomUniform urand;
    std::vector<vec2> res(points_initial_size);

    const int plat_count_min = points_initial_size / 8;
    const int plat_count_max = points_initial_size / 3;

    const GLfloat point_dist_min = frame_width / points_initial_size * 4.f;
    const GLfloat point_dist_max = frame_width / points_initial_size * 5.f;

    const GLfloat plat_min_width = 30.f;
    const GLfloat plat_max_width = 50.f;

    size_t platforms_count = urand.generateu(plat_count_min, plat_count_max);
    platforms.reserve(platforms.size() + platforms_count);
    std::vector<GLuint> plat_idx(platforms_count, 0);


    urand.fill_unique<GLuint>(plat_idx.begin(), plat_idx.end(), 0, res.size());

    GLfloat plat_width;
    const GLfloat deviation = 20.f;
    for (size_t i = 0; i < res.size(); ++i) {
        if (i == 0) {
            res[i].x = initial_p.x;
            res[i].y = urand.generaten(initial_p.y, deviation);
        } else {
            if (std::count(plat_idx.begin(), plat_idx.end(), i) != 0) {
                // platform case
                plat_width = urand.generateu(plat_min_width, plat_max_width);
                res[i].x = res[i - 1].x + urand.generateu(point_dist_min,
                                                          point_dist_max);
                res[i].y = urand.generaten(res[i - 1].y, deviation)
                           + urand.generateu(-deviation, deviation);
            } else if (std::count(plat_idx.begin(),
                                  plat_idx.end(), i - 1) != 0) {
                // prev. platform case
                res[i].x = res[i - 1].x + plat_width;
                res[i].y = res[i - 1].y;
            } else { // normal case
                res[i].x = res[i - 1].x + urand.generateu(point_dist_min,
                                                          point_dist_max);
                res[i].y = urand.generaten(res[i - 1].y, deviation)
                           + urand.generateu(-deviation, deviation);
            }
        }
    }

    return res;
}

LevelGenerator::LevelGenerator()
{
    height_min = utils::getScreenHeight<GLfloat>()
                 - utils::getScreenHeight<GLfloat>() / 2;
    height_max = utils::getScreenHeight<GLfloat>();
}

void
LevelGenerator::extendToRight(std::vector<vec2>& points, std::vector<vec2>& stars)
{
    auto right = generate_lines(points[points.size() - 1]);

    GLfloat old_right = points[points.size() - 1].x;
    GLfloat line_length = std::abs(right[right.size() - 1].x
                                   - points[points.size() - 1].x);

    points.reserve(points.size() + right.size());
    points.insert(points.end(), right.begin(), right.end());

    GLfloat old_max_x = old_right;
    auto new_stars = generate_stars(old_max_x, old_max_x + line_length);
    stars.reserve(stars.size() + new_stars.size());
    stars.insert(stars.end(), new_stars.begin(), new_stars.end());
}

void
LevelGenerator::extendToLeft(std::vector<vec2>& points, std::vector<vec2>& stars)
{
    GLfloat old_left = points[0].x;
    auto left = generate_lines(points[0]);

    GLfloat line_length = std::abs(points[0].x - left[left.size() - 1].x);

    GLfloat initial_x = -left[left.size() - 1].x;
    std::for_each(left.begin(), left.end(),
                  [initial_x](vec2& p){ p.x += initial_x; });

    points.insert(points.end(), left.begin(), left.end());
    std::rotate(points.begin(), points.begin() + left.size(), points.end());

    GLfloat old_max_x = old_left;
    auto new_stars = generate_stars(old_max_x - line_length, old_max_x);
    stars.reserve(stars.size() + new_stars.size());
    stars.insert(stars.end(), new_stars.begin(), new_stars.end());
}
#include <geometry.hpp>
#include "models/level.hpp"
#include "utils.hpp"

const int stars_initial_size = 200;
const int points_initial_size = 100;

using Utils::Point;

Level::~Level()
{

}

std::vector<Point> Level::generate_lines(int initial_x)
{
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

Level::Level(Camera* camera) : Drawable(camera), surfaceType(SurfaceType::MOON),
                               camera(camera), points_count(points_initial_size), stars_count(stars_initial_size)
{
    height_min = Utils::getScreenHeight<int>() - Utils::getScreenHeight<int>() / 2;
    height_max = Utils::getScreenHeight<int>() - Utils::getScreenHeight<int>() / 5;

    points = generate_lines(0);


    stars.resize(stars_count);

    Utils::RandomUniform urand;

    for (auto& star: stars) {
        star.x = urand.generate(0, Utils::getScreenWidth<int>());
        star.y = urand.generate(0, height_max);
    }
}

void Level::extendToRight()
{
    auto right = generate_lines(points[points.size() - 1].x);
    std::vector<Point> res;
    res.reserve(right.size() + points.size());
    res.insert(res.end(), points.begin(), points.end());
    res.insert(res.end(), right.begin(), right.end());

    points = res;

    size_t old_size = stars.size();
    int old_max_x = stars[stars.size() - 1].x;
    stars.resize(stars.size() + stars_initial_size);
    for (size_t i = old_size; i < stars.size(); ++i) {
        stars[i].x = urand.generate(old_max_x, old_max_x + Utils::getScreenWidth<int>());
        stars[i].y = urand.generate(0, height_max);
    }
}

void Level::extendToLeft()
{
    auto left = generate_lines(0);
    GLfloat initial_x = -left[left.size() - 1].x;
    for (auto& el: left)
        el.x += initial_x;

    std::vector<Point> res;
    res.reserve(left.size() + points.size());
    res.insert(res.end(), left.begin(), left.end());
    res.insert(res.end(), points.begin(), points.end());

    points = res;

    size_t old_size = stars.size();
    int old_max_x = stars[0].x;
    stars.resize(stars.size() + stars_initial_size);
    for (size_t i = old_size; i < stars.size(); ++i) {
        stars[i].x = urand.generate(old_max_x - Utils::getScreenWidth<int>(), old_max_x);
        stars[i].y = urand.generate(0, height_max);
    }
}

void Level::render(MoonLanderProgram& program)
{
    auto shifted_points = Utils::moveVertices(points, -camera->getX(), -camera->getY());
    auto shifted_stars = Utils::moveVertices(stars, -camera->getX(), -camera->getY());
    Geometry::drawLinen(shifted_points);
    Geometry::drawDots(shifted_stars);
}

bool Level::hasCollision(Utils::Rect coord, GLfloat angle)
{
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

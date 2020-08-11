#include <geometry.hpp>
#include "models/level.hpp"
#include "utils.hpp"

const int POINT_COUNT = 60;
const int STARS_COUNT = 200;

const int PLATFORM_COUNT_MIN = POINT_COUNT / 8;
const int PLATFORM_COUNT_MAX = POINT_COUNT / 3;

Level::~Level()
{

}

Level::Level(Camera* camera) : Model(camera), surfaceType(SurfaceType::MOON),
                 points(POINT_COUNT), stars(STARS_COUNT),
                 camera(camera)
{
    Utils::RandomUniform urand;

    const int HEIGHT_MIN = Utils::getScreenHeight() - Utils::getScreenHeight() / 2;
    const int HEIGHT_MAX = Utils::getScreenHeight() - Utils::getScreenHeight() / 5;

    const int frame_width = Utils::getScreenWidth() * 2;
    const int POINT_DISTANCE_MIN = frame_width / POINT_COUNT - (frame_width / POINT_COUNT) / 2;
    const int POINT_DISTANCE_MAX = frame_width / POINT_COUNT;

    const int PLATFORM_MIN_WIDTH = POINT_DISTANCE_MIN / 2.f;
    const int PLATFORM_MAX_WIDTH = POINT_DISTANCE_MIN / 1.1f;

    platforms_count = urand.generate(PLATFORM_COUNT_MIN, PLATFORM_COUNT_MAX);

    std::mt19937 generator;
    std::vector<GLuint> plat_idx(platforms_count, 0);
    std::shuffle(plat_idx.begin(), plat_idx.end(), generator);
    points.reserve(POINT_COUNT + platforms_count);
    GLfloat prev_width;
    for (int i = 0, p = 0; i < points.size(); ++i) {
        if (p == 0) {
            points[i].x = 0;
            points[i].y = urand.generate(HEIGHT_MIN, HEIGHT_MAX);
            ++p;
        } else {
            if (std::find(plat_idx.begin(), plat_idx.end(), p) != plat_idx.end()) { // platform case
                prev_width = urand.generate(PLATFORM_MIN_WIDTH, PLATFORM_MAX_WIDTH);
                points[i].x = points[i - 1].x + urand.generate(POINT_DISTANCE_MIN, POINT_DISTANCE_MAX) - prev_width / 2;
                points[i].y = urand.generate(HEIGHT_MIN, HEIGHT_MAX);
            } else if (std::find(plat_idx.begin(), plat_idx.end(), p - 1) != plat_idx.end()) { // prev. platform case
                points[i].x = points[i - 1].x + prev_width / 2;
                points[i].y = points[i - 1].y;
                ++p;
            } else { // normal case
                points[i].x = points[i - 1].x + urand.generate(POINT_DISTANCE_MIN, POINT_DISTANCE_MAX);
                points[i].y = urand.generate(HEIGHT_MIN, HEIGHT_MAX);
                ++p;
            }
        }
    }

    for (auto& star: stars) {
        star.x = urand.generate(0, Utils::getScreenWidth());
        star.y = urand.generate(0, HEIGHT_MAX);
    }
}

void Level::render(MoonLanderProgram& program)
{
    Geometry geometry;
    auto shifted_points = Utils::moveVertices(points, -camera->getX(), -camera->getY());
    auto shifted_stars = Utils::moveVertices(stars, -camera->getX(), -camera->getY());
    geometry.drawLinen(shifted_points);
    geometry.drawDots(shifted_stars, {255, 255, 255, 255});
}

int Level::renderSky()
{

    return 0;
}

void Level::setSurfaceType(SurfaceType surface)
{
    this->surfaceType = surface;
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

#include <vector>
#include <ctime>
#include <random>
#include <iostream>
#include <geometry.hpp>
#include "models/level.hpp"
#include "utils.hpp"

const int PLATFORM_MIN_WIDTH = 10;
const int PLATFORM_MAX_WIDTH = 20;

const int HEIGHT_MIN = 300;
const int HEIGHT_MAX = 400;

const int POINT_DISTANCE_MIN = 20;
const int POINT_DISTANCE_MAX = 50;

const int POINT_COUNT = 100;

const int PLATFORM_COUNT_MIN = POINT_COUNT / 10;
const int PLATFORM_COUNT_MAX = POINT_COUNT / 5;

Level::~Level()
{

}


void Level::render()
{
    Geometry geometry;
    for (int i = 0; i < points.size() - 1; ++i) {
        if (i % platform_between == 0 && i != 0) {
            geometry.drawLine(points.at(i).x - points.at(i).w / 2,
                              points.at(i).y, points.at(i).x + points.at(i).w / 2, points.at(i).y);
            geometry.drawLine(points.at(i).x + points.at(i).w / 2,
                              points.at(i).y, points.at(i + 1).x, points.at(i + 1).y);
        } else if ((i + 1) % platform_between == 0) {
            geometry.drawLine(points.at(i).x, points.at(i).y,
                              points.at(i + 1).x - points.at(i + 1).w / 2, points.at(i + 1).y);
        } else {
            geometry.drawLine(points.at(i).x, points.at(i).y,
                              points.at(i + 1).x, points.at(i + 1).y);
        }
    }
}

int Level::renderSky()
{

    return 0;
}

Level::Level() : surfaceType(SurfaceType::MOON), points(POINT_COUNT)
{
    Utils::RandomUniform urand;

    platforms_count = urand.generate(PLATFORM_COUNT_MIN, PLATFORM_COUNT_MAX);
    platform_between = POINT_COUNT / platforms_count;

    for (int i = 0; i < points.size(); ++i) {
        points.at(i).x = (i == 0) ? 0 : (points.at(i - 1).x
                                         + urand.generate(POINT_DISTANCE_MIN, POINT_DISTANCE_MAX));
        points.at(i).y = urand.generate(HEIGHT_MIN, HEIGHT_MAX);

        points.at(i).h = 1;
        points.at(i).w = i % platform_between == 0 ? urand.generate(PLATFORM_MIN_WIDTH, PLATFORM_MAX_WIDTH) : 0;
    }
}

void Level::setSurfaceType(SurfaceType surface)
{
    this->surfaceType = surface;
}

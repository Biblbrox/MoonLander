#ifndef MOONLANDER_LEVELGENERATOR_HPP
#define MOONLANDER_LEVELGENERATOR_HPP

#include <SDL_render.h>
#include <vector>
#include <moonlanderprogram.hpp>
#include <camera.hpp>
#include "types.hpp"

class LevelGenerator {
public:
    explicit LevelGenerator();
    ~LevelGenerator();
    void extendToRight(std::vector<Utils::Point>& points, std::vector<Utils::Point> stars);
    void extendToLeft(std::vector<Utils::Point>& points, std::vector<Utils::Point> stars);
    std::vector<Utils::Point> generate_lines(int initial_x) const;
    std::vector<Utils::Point> generate_stars() const;

private:
    int points_count;
    int stars_count;
    const int frame_width = 2000;

    int height_min;
    int height_max;
};


#endif //MOONLANDER_LEVELGENERATOR_HPP

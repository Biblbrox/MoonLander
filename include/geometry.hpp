#ifndef MOONLANDER_GEOMETRY_PPH
#define MOONLANDER_GEOMETRY_PPH

#include <vector>
#include "utils.hpp"
#include <GL/gl.h>

class Geometry {
public:
    void drawLinen(const std::vector<Utils::Point>& points,
            Utils::GLColor color = {1.f, 1.f, 1.f, 1.f});
    void drawDots(const std::vector<Utils::Point>& dots,
            Utils::GLColor color = {1.f, 1.f, 1.f, 1.f});
};

#endif //MOONLANDER_GEOMETRY_PPH

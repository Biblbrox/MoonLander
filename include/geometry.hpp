#ifndef MOONLANDER_GEOMETRY_PPH
#define MOONLANDER_GEOMETRY_PPH

#include <vector>
#include "utils.hpp"
#include <GL/gl.h>

class Geometry {
public:
    void drawLinen(const std::vector<Utils::Point>& points);
    void drawDots(const std::vector<Utils::Point>& dots);
};

#endif //MOONLANDER_GEOMETRY_PPH

#ifndef MOONLANDER_GEOMETRY_PPH
#define MOONLANDER_GEOMETRY_PPH

#include <vector>
#include "utils.hpp"
#include <GL/gl.h>

class Geometry {
public:
    static void drawLinen(const std::vector<utils::Point>& points);
    static void drawDots(const std::vector<utils::Point>& dots);
};

#endif //MOONLANDER_GEOMETRY_PPH

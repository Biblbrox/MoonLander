#ifndef MOONLANDER_GEOMETRY_PPH
#define MOONLANDER_GEOMETRY_PPH

#include <vector>
#include "utils.hpp"
#include <GL/gl.h>

class Geometry {
public:
    void drawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2,
            Utils::GLColor color = {1.f, 1.f, 1.f, 1.f});
    void drawLinen(const std::vector<Utils::Point>& points,
            Utils::GLColor color = {1.f, 1.f, 1.f, 1.f});
    void drawDot(GLfloat x, GLfloat y);
    void drawDots(const std::vector<Utils::Point>& dots,
            Utils::GLColor color = {1.f, 1.f, 1.f, 1.f});
};

#endif //MOONLANDER_GEOMETRY_PPH

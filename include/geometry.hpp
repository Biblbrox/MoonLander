#ifndef MOONLANDER_GEOMETRY_PPH
#define MOONLANDER_GEOMETRY_PPH

#include <GL/gl.h>
#include <vector>
#include "utils.hpp"

class Geometry {
public:
    void drawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2,
            SDL_Color color = {255, 255, 255, 255});
    void drawLinen(const std::vector<Utils::Rect>& points, SDL_Color color);
};

#endif //MOONLANDER_GEOMETRY_PPH

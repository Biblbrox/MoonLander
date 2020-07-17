#ifndef MOONLANDER_GEOMETRY_PPH
#define MOONLANDER_GEOMETRY_PPH


#include <GL/gl.h>
#include <vector>
#include "renderer.hpp"
#include "utils.hpp"

class Geometry {
public:
    void drawLine(GLuint x1, GLuint y1, GLuint x2, GLuint y2,
            SDL_Color color = {255, 255, 255, 255});
    void drawLinen(const std::vector<Utils::Rect>& points, SDL_Color color);
};


#endif //MOONLANDER_GEOMETRY_PPH

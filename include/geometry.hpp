#ifndef MOONLANDER_GEOMETRY_PPH
#define MOONLANDER_GEOMETRY_PPH


#include "renderer.hpp"

class Geometry {
public:
    void drawLine(Renderer& renderer,
                  int x1, int y1, int x2, int y2);
};


#endif //MOONLANDER_GEOMETRY_PPH

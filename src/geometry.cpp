#include "geometry.hpp"

void Geometry::drawLine(Renderer &renderer, int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(renderer.getRenderer(), x1, y1, x2, y2);
}

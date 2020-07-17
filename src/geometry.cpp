#include "geometry.hpp"

void Geometry::drawLine(GLuint x1, GLuint y1, GLuint x2, GLuint y2, SDL_Color color)
{
    glBegin(GL_LINES);
    glColor3f(color.r / 255.f, color.g / 255.f, color.b / 255.f);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void Geometry::drawLinen(const std::vector<Utils::Rect>& points, SDL_Color color)
{
    glBegin(GL_LINES);
    for (auto& point: points)
        glVertex2f(point.x, point.y);

    glEnd();
}

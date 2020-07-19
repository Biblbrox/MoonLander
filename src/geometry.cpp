#include <GL/glew.h>
#include "geometry.hpp"

void Geometry::drawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, SDL_Color color)
{
    static GLfloat vertices[4];
    vertices[0] = x1;
    vertices[1] = y1;
    vertices[2] = x2;
    vertices[3] = y2;

    static GLuint indices[4];
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 3;

    static GLuint verticesID = 0;
    static GLuint indicesID = 0;

    glGenBuffers(1, &verticesID);
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &indicesID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLfloat), indices, GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glVertexPointer(2, GL_FLOAT, 0, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, NULL);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Geometry::drawLinen(const std::vector<Utils::Rect>& points, SDL_Color color)
{
    glBegin(GL_LINES);
    for (auto& point: points)
        glVertex2f(point.x, point.y);

    glEnd();
}

#include <GL/glew.h>
#include "geometry.hpp"

void Geometry::drawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, Utils::GLColor color)
{
    static GLfloat vertices[4];
    vertices[0] = x1;
    vertices[1] = y1;
    vertices[2] = x2;
    vertices[3] = y2;

    static GLuint indices[2];
    indices[0] = 0;
    indices[1] = 1;

    static GLuint verticesID = 0;
    static GLuint indicesID = 0;

    glGenBuffers(1, &verticesID);
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &indicesID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glVertexPointer(2, GL_FLOAT, 0, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, nullptr);
    glDisableClientState(GL_VERTEX_ARRAY);

    glDeleteBuffers(1, &verticesID);
    glDeleteBuffers(1, &indicesID);
}

void Geometry::drawLinen(const std::vector<Utils::Point>& points, Utils::GLColor color)
{
    glColor3f(color.r, color.g, color.b);

    auto vertices = new Utils::Point[points.size()];

    for (int i = 0; i < points.size(); ++i) {
        vertices[i].x = points[i].x;
        vertices[i].y = points[i].y;
    }

    GLuint verticesID = 0;

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &verticesID);
    glBindVertexArray(VAO);

    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Utils::Point) * points.size(), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_LINE_STRIP, 0, points.size());
    glDisableVertexAttribArray(0);
    glDisableClientState(GL_VERTEX_ARRAY);

    glDeleteBuffers(1, &verticesID);
    glDeleteVertexArrays(1, &VAO);
    delete[] vertices;
}

void Geometry::drawDot(GLfloat x, GLfloat y)
{
    GLfloat vertices[2];
    vertices[0] = x;
    vertices[1] = y;
    GLuint indices[1];
    indices[0] = 0;

    GLuint verticesID = 0;
    GLuint indicesID = 0;
    glGenBuffers(1, &verticesID);
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &indicesID);
    glBindBuffer(GL_ARRAY_BUFFER, indicesID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint), indices, GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glVertexPointer(2, GL_FLOAT, 0, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
    glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, nullptr);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDeleteBuffers(1, &verticesID);
    glDeleteBuffers(1, &indicesID);
}

void Geometry::drawDots(const std::vector<Utils::Point>& dots, Utils::GLColor color)
{
    glColor3f(color.r, color.g, color.b);

    auto vertices = new Utils::Point[dots.size()];

    for (int i = 0; i < dots.size(); ++i) {
        vertices[i].x = dots[i].x;
        vertices[i].y = dots[i].y;
    }

    GLuint VAO;
    GLuint verticesID = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &verticesID);
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Utils::Point) * dots.size(), vertices, GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_POINTS, 0, dots.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableVertexAttribArray(0);

    glDeleteBuffers(1, &verticesID);
    glDeleteVertexArrays(1, &VAO);
    delete[] vertices;
}

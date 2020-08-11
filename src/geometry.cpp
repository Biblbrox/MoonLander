#include <GL/glew.h>
#include "geometry.hpp"

void Geometry::drawLinen(const std::vector<Utils::Point>& points, Utils::GLColor color)
{
    auto vertices = points.data();

    GLuint verticesID = 0;
    GLuint VAO = 0;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &verticesID);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Utils::Point) * points.size(), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_LINE_STRIP, 0, points.size());
    glDisableVertexAttribArray(0);

    glDeleteBuffers(1, &verticesID);
    glDeleteVertexArrays(1, &VAO);
}

void Geometry::drawDots(const std::vector<Utils::Point>& dots, Utils::GLColor color)
{
    auto vertices = dots.data();

    GLuint VAO = 0;
    GLuint verticesID = 0;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &verticesID);
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Utils::Point) * dots.size(), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_POINTS, 0, dots.size());
    glDisableVertexAttribArray(0);

    glDeleteBuffers(1, &verticesID);
    glDeleteVertexArrays(1, &VAO);
}

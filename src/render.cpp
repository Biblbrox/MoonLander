#include "../include/render.hpp"

using glm::vec2;
using glm::mat4;
using glm::vec3;
using utils::rotate_around;

void render::drawLinen(const std::vector<vec2>& points, bool adjacency)
{
    auto vertices = points.data();

    GLuint verticesID = 0;
    GLuint VAO = 0;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &verticesID);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * points.size(), vertices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    if (adjacency) {
        glDrawArrays(GL_LINE_STRIP_ADJACENCY, 0, points.size());
    } else {
        assert(points.size() % 2 == 0);
        glDrawArrays(GL_LINES, 0, points.size());
    }


    glDisableVertexAttribArray(0);

    glDeleteBuffers(1, &verticesID);
    glDeleteVertexArrays(1, &VAO);
}

void render::drawDots(const std::vector<vec2>& dots)
{
    auto vertices = dots.data();

    GLuint VAO = 0;
    GLuint verticesID = 0;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &verticesID);
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * dots.size(), vertices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_POINTS, 0, dots.size());
    glDisableVertexAttribArray(0);

    glDeleteBuffers(1, &verticesID);
    glDeleteVertexArrays(1, &VAO);
}

void
render::drawSprite(std::shared_ptr<MoonLanderProgram> program, Texture &texture,
                   GLfloat x,
                   GLfloat y, GLfloat angle, GLfloat scale_factor)
{
    program->switchToTriangles();
    program->setTextureRendering(true);

    if (texture.getVAO() != 0) {
        GLfloat half = texture.getWidth() / 2.f;
        mat4 rotation = rotate_around(mat4(1.f),vec3(x + half,y + half,0.f), angle);
        mat4 translation = translate(mat4(1.f), vec3(x, y, 0.f));
        mat4 scaling = scale(mat4(1.f), vec3(scale_factor, scale_factor, 1.f));
        program->leftMultModel(scaling * rotation * translation);
        program->updateModel();

        glBindTexture(GL_TEXTURE_2D, texture.getTextureID());
        glBindVertexArray(texture.getVAO());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);

        translation = translate(mat4(1.f), vec3(-x, -y, 0.f));
        rotation = rotate_around(mat4(1.f),vec3(x + half,y + half,0.f), -angle);
        scaling = scale(mat4(1.f),vec3(1 / scale_factor, 1 / scale_factor, 1.f));
        program->leftMultModel(translation * rotation * scaling);
        program->updateModel();
    }
}

void render::drawTriangles(const std::vector<vec2>& points)
{
    assert(points.size() % 3 == 0);
    auto vertices = points.data();

    GLuint verticesID = 0;
    GLuint VAO = 0;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &verticesID);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * points.size(), vertices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, points.size());
    glDisableVertexAttribArray(0);

    glDeleteBuffers(1, &verticesID);
    glDeleteVertexArrays(1, &VAO);
}

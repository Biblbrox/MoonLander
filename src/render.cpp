#include "../include/render.hpp"

using glm::vec2;

void render::drawLinen(const std::vector<vec2>& points)
{
    assert(points.size() % 2 == 0 && "Points must be part of lines");
    assert(points.size() % 4 == 0);
    auto vertices = points.data();

    GLuint verticesID = 0;
    GLuint VAO = 0;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &verticesID);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * points.size(), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_LINE_STRIP_ADJACENCY, 0, points.size());
    //glDrawArrays(GL_LINE_STRIP, 0, points.size());
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * dots.size(), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_POINTS, 0, dots.size());
    glDisableVertexAttribArray(0);

    glDeleteBuffers(1, &verticesID);
    glDeleteVertexArrays(1, &VAO);
}

void render::drawLevel(std::shared_ptr<MoonLanderProgram> program,
            std::vector<vec2> &points,
            std::vector<vec2> &stars, GLfloat scale_factor)
{
    program->setTextureRendering(false);

    glm::mat4 scaling = glm::scale(glm::mat4(1.f),
                                   glm::vec3(scale_factor, scale_factor,1.f));
    program->leftMultModel(scaling);
    program->updateModel();

    program->switchToLines();
    drawLinen(points);

    program->switchToPoints();
    drawDots(stars);

    scaling = glm::scale(glm::mat4(1.f),
                         glm::vec3(1 / scale_factor, 1 / scale_factor,1.f));
    program->leftMultModel(scaling);
    program->updateModel();
}

void render::drawSprite(std::shared_ptr<MoonLanderProgram> program, Texture &texture,
                GLfloat x,
                GLfloat y, GLfloat angle, GLfloat scale_factor)
{
    using glm::mat4;
    using glm::vec3;

    program->switchToTriangles();
    program->setTextureRendering(true);

    if (texture.getVAO() != 0) {
        mat4 rotation = utils::rotate_around(mat4(1.f),
                                             vec3(x +
                                                  texture.getWidth() / 2.f,
                                                  y +
                                                  texture.getHeight() / 2.f,
                                                  0.f), angle);
        mat4 translation = translate(mat4(1.f), vec3(x, y, 0.f));
        mat4 scaling = scale(mat4(1.f),
                             vec3(scale_factor, scale_factor, 1.f));
        program->leftMultModel(scaling * rotation * translation);
        program->updateModel();

        glBindTexture(GL_TEXTURE_2D, texture.getTextureID());
        glBindVertexArray(texture.getVAO());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);

        translation = translate(mat4(1.f), vec3(-x, -y, 0.f));
        rotation = utils::rotate_around(mat4(1.f),
                                        vec3(x + texture.getWidth() / 2.f,
                                             y + texture.getHeight() / 2.f,
                                             0.f), -angle);
        scaling = scale(mat4(1.f),
                        vec3(1 / scale_factor, 1 / scale_factor, 1.f));
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * points.size(), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, points.size());
    glDisableVertexAttribArray(0);

    glDeleteBuffers(1, &verticesID);
    glDeleteVertexArrays(1, &VAO);
}

void render::drawLinenTriangulation(const std::vector<vec2> &points)
{
    using glm::vec2;
    using glm::normalize;
    using glm::dot;

    const GLfloat thickness = 5.f;

    assert(points.size() % 2 == 0);
    std::vector<vec2> trVertices;
    for (size_t i = 0; i < points.size() - 2; i += 2) {
        vec2 P0 = vec2(points[i].x, points[i].y);
        vec2 P1 = points[i + 1];
        vec2 P2 = points[i + 2];

        vec2 lineP0 = P0 - P1;
        vec2 lineP1 = P1 - P0;
        vec2 lineP2 = P2 - P1;

        vec2 normalP0 = normalize(vec2(-lineP0.y, lineP0.x));
        vec2 normalP1 = normalize(vec2(-lineP1.y, lineP1.x));
        vec2 normalP2 = normalize(vec2(-lineP2.y, lineP2.x));

        vec2 tangent = normalize(normalize(lineP2) + normalize(lineP1));
        vec2 milter = vec2(-tangent.y, tangent.x);

        float length = thickness / dot(milter, normalP1);

        vec2 a = P0 + thickness * normalP0;
        vec2 b = P0 - thickness * normalP0;
        vec2 c = P1 + length;
        vec2 d = P1 - length;
        vec2 e = P2 + thickness * normalP2;
        vec2 f = P2 - thickness * normalP2;

        std::vector<vec2> first;
        first.push_back(a);
        first.push_back(b);
        first.push_back(c);
        std::vector<vec2> second;
        second.push_back(c);
        second.push_back(b);
        second.push_back(d);
        std::vector<vec2> third;
        third.push_back(e);
        third.push_back(c);
        third.push_back(d);
        std::vector<vec2> fourth;
        fourth.push_back(e);
        fourth.push_back(d);
        fourth.push_back(f);
        drawTriangles(first);
        drawTriangles(second);
        drawTriangles(third);
        drawTriangles(fourth);

        trVertices.push_back(a);
        trVertices.push_back(b);
        trVertices.push_back(c);
        trVertices.push_back(d);
        trVertices.push_back(e);
        trVertices.push_back(f);
    }
}

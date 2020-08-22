#ifndef MOONLANDER_RENDERER_H
#define MOONLANDER_RENDERER_H

#include <GL/glew.h>
#include "moonlanderprogram.hpp"
#include "sprite.hpp"
#include "geometry.hpp"

class Renderer {
public:

    static void render(MoonLanderProgram& program, Texture& texture, GLfloat x, GLfloat y, GLfloat angle)
    {
        if (texture.getVAO() != 0) {
            glm::mat4 transform = Utils::rotateAround(glm::mat4(1.f),
                                                      glm::vec3(x + texture.getWidth() / 2.f,
                                                                y + texture.getHeight() / 2.f, 0.f), angle);
            program.leftMultModel(glm::translate(transform, glm::vec3(x, y, 0.f)));
            program.updateModel();

            glBindTexture(GL_TEXTURE_2D, texture.getTextureID());
            glBindVertexArray(texture.getVAO());
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);

            transform = glm::translate(glm::mat4(1.f), glm::vec3(-x, -y, 0.f));
            program.leftMultModel(Utils::rotateAround(transform,
                                                      glm::vec3(x + texture.getWidth() / 2.f,
                                                                y + texture.getHeight() / 2.f, 0.f), -angle));
            program.updateModel();
        }
    }

    static void render(MoonLanderProgram& program,
                       std::vector<Utils::Point>& points, std::vector<Utils::Point>& stars)
    {
        //auto shifted_points = Utils::moveVertices(points, -camera.getX(), -camera.getY());
        //auto shifted_stars = Utils::moveVertices(stars, -camera.getX(), -camera.getY());
        Geometry::drawLinen(points);
        Geometry::drawDots(stars);
    }
};

#endif //MOONLANDER_RENDERER_H

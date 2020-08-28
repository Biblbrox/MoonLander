#ifndef MOONLANDER_RENDERER_HPP
#define MOONLANDER_RENDERER_HPP

#include <GL/glew.h>
#include "moonlanderprogram.hpp"
#include "sprite.hpp"
#include "geometry.hpp"

class Renderer {
public:

    static void render(MoonLanderProgram& program, Texture& texture, GLfloat x,
                       GLfloat y, GLfloat angle, GLfloat scale_factor)
    {
        if (texture.getVAO() != 0) {
            glm::mat4 rotation = utils::rotate_around(glm::mat4(1.f),
                                                      glm::vec3(x + texture.getWidth() / 2.f,
                                                                y + texture.getHeight() / 2.f, 0.f), angle);
            glm::mat4 translation = glm::translate(glm::mat4(1.f), glm::vec3(x, y, 0.f));
            glm::mat4 scaling = glm::scale(glm::mat4(1.f), glm::vec3(scale_factor, scale_factor, 1.f));
            program.leftMultModel(scaling * rotation * translation);
            program.updateModel();

            glBindTexture(GL_TEXTURE_2D, texture.getTextureID());
            glBindVertexArray(texture.getVAO());
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);

            translation = glm::translate(glm::mat4(1.f), glm::vec3(-x, -y, 0.f));
            rotation = utils::rotate_around(glm::mat4(1.f), glm::vec3(x + texture.getWidth() / 2.f,
                                                                     y + texture.getHeight() / 2.f, 0.f), -angle);
            scaling = glm::scale(glm::mat4(1.f), glm::vec3(1 / scale_factor, 1 / scale_factor, 1.f));
            program.leftMultModel(translation * rotation * scaling);
            program.updateModel();
        }
    }

    static void render(MoonLanderProgram& program,
                       std::vector<utils::Point>& points, std::vector<utils::Point>& stars, GLfloat scale_factor)
    {
        glm::mat4 scaling = glm::scale(glm::mat4(1.f), glm::vec3(scale_factor, scale_factor, 1.f));
        program.leftMultModel(scaling);
        program.updateModel();

        Geometry::drawLinen(points);
        Geometry::drawDots(stars);

        scaling = glm::scale(glm::mat4(1.f), glm::vec3(1 / scale_factor, 1 / scale_factor, 1.f));
        program.leftMultModel(scaling);
        program.updateModel();
    }
};

#endif //MOONLANDER_RENDERER_HPP

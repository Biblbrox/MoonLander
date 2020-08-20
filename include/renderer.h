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

//    static void render(MoonLanderProgram& program, Sprite& sprite, GLfloat x, GLfloat y, GLfloat angle)
//    {
//        if (sprite.getVAO() != nullptr) {
//            glm::mat4 transform = Utils::rotateAround(glm::mat4(1.f),
//                                                      glm::vec3(x + sprite.getCurrentClip().w / 2.f,
//                                                                y + sprite.getCurrentClip().h / 2.f, 0.f), angle);
//            program.leftMultModel(glm::translate(transform, glm::vec3(x, y, 0.f)));
//            program.updateModel();
//
//            glBindTexture(GL_TEXTURE_2D, sprite.getTextureID());
//            glBindVertexArray(sprite.getVAO()[sprite.getIdx()]);
//            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
//            glBindTexture(GL_TEXTURE_2D, 0);
//            glBindVertexArray(0);
//
//            transform = glm::translate(glm::mat4(1.f), glm::vec3(-x, -y, 0.f));
//            program.leftMultModel(Utils::rotateAround(transform,
//                                                      glm::vec3(x + sprite.getCurrentClip().w / 2.f,
//                                                                y + sprite.getCurrentClip().h / 2.f, 0.f), -angle));
//            program.updateModel();
//        }
//    }
//
    static void render(MoonLanderProgram& program, Level& level, Camera& camera)
    {
        auto shifted_points = Utils::moveVertices(level.points, -camera.getX(), -camera.getY());
        auto shifted_stars = Utils::moveVertices(level.stars, -camera.getX(), -camera.getY());
        Geometry::drawLinen(shifted_points);
        Geometry::drawDots(shifted_stars);
    }
//
//    static void render(MoonLanderProgram& program, TextTexture& texture, GLfloat x, GLfloat y, GLfloat angle, GLfloat scale_f)
//    {
//        if (texture.getTextureID() != 0) {
//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_2D, texture.getTextureID());
//            program.setTexture(0);
//            program.setModel(glm::translate(program.getModel(), glm::vec3(x, y, 0.f)));
//            program.updateModel();
//
//            program.setModel(glm::scale(program.getModel(), glm::vec3(scale_f, scale_f, 1.f)));
//            program.updateModel();
//            glBindVertexArray(texture.getVAO());
//            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
//            glBindTexture(GL_TEXTURE_2D, 0);
//            glBindVertexArray(0);
//            program.setModel(glm::scale(program.getModel(), glm::vec3(1 / scale_f, 1 / scale_f, 1.f)));
//            program.updateModel();
//
//            program.setModel(glm::translate(program.getModel(), glm::vec3(-x, -y, 0.f)));
//            program.updateModel();
//        }
//    }
};

#endif //MOONLANDER_RENDERER_H

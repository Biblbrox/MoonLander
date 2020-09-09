#ifndef MOONLANDER_RENDER_HPP
#define MOONLANDER_RENDER_HPP

#include <GL/glew.h>
#include "moonlanderprogram.hpp"
#include "sprite.hpp"

using glm::vec2;

namespace render {

    /**
     * Render some subclass of Texture.
     * @param program
     * @param texture
     * @param x
     * @param y
     * @param angle
     * @param scale_factor
     */
    void drawSprite(std::shared_ptr<MoonLanderProgram> program, Texture &texture,
                    GLfloat x,
                    GLfloat y, GLfloat angle, GLfloat scale_factor);

    void drawLinen(const std::vector<vec2>& points);

    void drawDots(const std::vector<vec2>& dots);

    void drawTriangles(const std::vector<vec2> &points);
};

#endif //MOONLANDER_RENDER_HPP

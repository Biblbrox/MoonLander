#ifndef MOONLANDER_TEXTURE_HPP
#define MOONLANDER_TEXTURE_HPP

#include <string>
#include <SDL_render.h>
#include <GL/gl.h>
//#include "freelable.hpp"
//#include "utils.hpp"

/**
 * Texture class.
 * This is only base methods. You can't create instance of this class
 */
class Texture {
public:
    virtual GLuint getTextureID();

    virtual GLuint getWidth() const;
    virtual GLuint getHeight() const;

    virtual void setWidth(GLuint width);
    virtual void setHeight(GLuint height);
    virtual void setX(GLfloat x);
    virtual void setY(GLfloat x);

    virtual GLfloat getX() const;
    virtual GLfloat getY() const;

protected:
    explicit Texture();

    GLuint textureID;

    GLuint texture_width = 0;
    GLuint texture_height = 0;

    GLfloat x = 0;
    GLfloat y = 0;
};

#endif //MOONLANDER_TEXTURE_HPP

#ifndef MOONLANDER_TEXTURE_HPP
#define MOONLANDER_TEXTURE_HPP

#include <string>
#include <SDL_render.h>
#include "freelable.hpp"
#include "renderer.hpp"
#include "utils.hpp"

class Texture {
public:
    explicit Texture();
    virtual ~Texture();

    virtual void loadTextureFromPixels32(GLuint* pixels, GLuint width, GLuint height);
    virtual void render(GLfloat x, GLfloat y, Utils::Rect *clip, double angle);
    virtual void render(GLfloat x, GLfloat y, Utils::Rect* clip);
    virtual void initVBO();
    virtual void freeVBO();

    virtual void free();

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
    GLuint textureID;

    GLuint texture_width = 0;
    GLuint texture_height = 0;

    GLfloat x_ = 0;
    GLfloat y_ = 0;

    GLuint vbo_id;
    GLuint ibo_id;
};

#endif //MOONLANDER_TEXTURE_HPP

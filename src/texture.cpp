#include <GL/glew.h>
#include "../include/texture.hpp"

Texture::Texture()
{
    textureID = 0;

    texture_width = 0;
    texture_height = 0;
}

GLuint Texture::getWidth() const
{
    return texture_width;
}

GLuint Texture::getHeight() const
{
    return texture_height;
}

GLfloat Texture::getX() const
{
    return x;
}

GLfloat Texture::getY() const
{
    return y;
}

void Texture::setX(GLfloat x_)
{
    x = x_;
}

void Texture::setY(GLfloat y_)
{
    y = y_;
}

void Texture::setWidth(GLuint width)
{
    texture_width = width;
}

void Texture::setHeight(GLuint height)
{
    texture_height = height;
}

GLuint Texture::getTextureID()
{
    return textureID;
}

void Texture::freeTexture()
{
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    texture_width = texture_height = 0;
}

Texture::~Texture()
{
    freeTexture();
}


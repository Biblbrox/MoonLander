#include <GL/glew.h>
#include "../include/texture.hpp"

Texture::Texture() : m_textureId(0), m_textureWidth(0), m_textureHeight(0)
{
}

GLuint Texture::getWidth() const
{
    return m_textureWidth;
}

GLuint Texture::getHeight() const
{
    return m_textureHeight;
}

GLuint Texture::getTextureID() const
{
    return m_textureId;
}

void Texture::freeTexture()
{
    if (m_textureId != 0) {
        glDeleteTextures(1, &m_textureId);
        m_textureId = 0;
    }

    m_textureWidth = m_textureHeight = 0;
}

Texture::~Texture()
{
    freeTexture();
}


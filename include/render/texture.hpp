#ifndef MOONLANDER_TEXTURE_HPP
#define MOONLANDER_TEXTURE_HPP

#include <string>
#include <GL/glew.h>

/**
 * Texture class.
 * This is only base methods. You can't create instance of this class
 */
class Texture
{
public:
    virtual ~Texture();

    virtual GLuint getTextureID() const;

    virtual GLuint getWidth() const noexcept;
    virtual GLuint getHeight() const noexcept;

    virtual GLuint getVAO() const = 0;
    virtual void generateDataBuffer() = 0;

    virtual void freeTexture() final;
    virtual void freeVBO() noexcept = 0;

protected:
    explicit Texture();

    GLuint m_textureId;

    GLuint m_textureWidth = 0;
    GLuint m_textureHeight = 0;
};

#endif //MOONLANDER_TEXTURE_HPP

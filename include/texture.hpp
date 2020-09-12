#ifndef MOONLANDER_TEXTURE_HPP
#define MOONLANDER_TEXTURE_HPP

#include <string>
#include <SDL_render.h>
#include <GL/glew.h>

/**
 * Texture class.
 * This is only base methods. You can't create instance of this class
 */
class Texture
{
public:
    virtual ~Texture();

    virtual GLuint getTextureID();

    virtual GLuint getWidth() const;
    virtual GLuint getHeight() const;

    virtual GLuint getVAO() const = 0;
    virtual void generateDataBuffer() = 0;

    virtual void freeTexture() final;
    virtual void freeVBO() = 0;

protected:
    explicit Texture();

    GLuint textureID;

    GLuint texture_width = 0;
    GLuint texture_height = 0;
};

#endif //MOONLANDER_TEXTURE_HPP

#ifndef MOONLANDER_TEXTURE_HPP
#define MOONLANDER_TEXTURE_HPP

#include <string>
#include <SDL_render.h>
#include <SDL_opengl.h>
#include "freelable.hpp"
#include "renderer.hpp"
#include "utils.hpp"

class Texture : public Freelable {
public:
    explicit Texture();
    virtual ~Texture();

    virtual void loadTextureFromPixels32(GLuint* pixels, GLuint width, GLuint height);
    virtual void renderTexture(int x, int y, Utils::Rect *clip = nullptr);
    virtual void renderTexture(SDL_Rect& dst,
                       SDL_Rect* clip);
    virtual void renderTexture(int x, int y, SDL_Rect *clip, double angle,
                       SDL_Point *center, SDL_RendererFlip flip);
    virtual void render(GLfloat x, GLfloat y, Utils::Rect* clip);

    virtual void free() override;

    virtual GLuint getTextureID();

    virtual GLuint getWidth() const;
    virtual GLuint getHeight() const;

    virtual void setWidth(int x);
    virtual void setHeight(int x);
    virtual void setX(int x);
    virtual void setY(int x);

    virtual int getX() const;
    virtual int getY() const;

protected:
    GLuint textureID;

    GLuint texture_width = 0;
    GLuint texture_height = 0;

    int x_ = 0;
    int y_ = 0;
};

#endif //MOONLANDER_TEXTURE_HPP

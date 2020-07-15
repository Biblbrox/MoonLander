#ifndef MOONLANDER_TEXTURE_HPP
#define MOONLANDER_TEXTURE_HPP

#include <string>
#include <SDL_render.h>
#include "freelable.hpp"
#include "renderer.hpp"

class Texture : public Freelable {
public:
    explicit Texture(Renderer* renderer);
    virtual ~Texture();

    virtual void renderTexture(int x, int y, SDL_Rect *clip);
    virtual void renderTexture(SDL_Rect& dst,
                       SDL_Rect* clip);

    virtual void free() override;

    virtual int getWidth() const;
    virtual int getHeight() const;

    virtual void setWidth(int x);
    virtual void setHeight(int x);
    virtual void setX(int x);
    virtual void setY(int x);

    virtual int getX() const;
    virtual int getY() const;

protected:
    Renderer* renderer_;
    SDL_Texture* texture_ = nullptr;
    int width_ = 0;
    int height_ = 0;
    int x_ = 0;
    int y_ = 0;
};

#endif //MOONLANDER_TEXTURE_HPP

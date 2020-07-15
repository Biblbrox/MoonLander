#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../include/texture.hpp"

Texture::~Texture()
{
    if (this->texture)
        SDL_DestroyTexture(this->texture);
}

void Texture::renderTexture(int x_, int y_, SDL_Rect *clip)
{
    SDL_Rect dst;
    dst.x = x_;
    dst.y = y_;
    if (clip != nullptr) {
        dst.w = clip->w;
        dst.h = clip->h;
    } else {
        SDL_QueryTexture(this->texture, nullptr, nullptr, &dst.w, &dst.h);
    }

    this->renderTexture(dst, clip);
}

void Texture::renderTexture(SDL_Rect &dst, SDL_Rect *clip)
{
    SDL_RenderCopy(this->renderer, this->texture, clip, &dst);
}

void Texture::free()
{
    if (this->texture)
        SDL_DestroyTexture(this->texture);

    this->texture = nullptr;
}

Texture::Texture(Renderer &renderer)
{
    this->renderer = renderer.getRenderer();
}

int Texture::getWidth() const
{
    return this->width;
}

int Texture::getHeight() const
{
    return this->height;
}

int Texture::getX() const
{
    return this->x;
}

int Texture::getY() const
{
    return this->y;
}


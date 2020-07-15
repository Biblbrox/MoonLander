#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../include/texture.hpp"

Texture::~Texture()
{
    if (texture_)
        SDL_DestroyTexture(texture_);
}

void Texture::renderTexture(int x, int y, SDL_Rect *clip)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (clip != nullptr) {
        dst.w = clip->w;
        dst.h = clip->h;
    } else {
        SDL_QueryTexture(texture_, nullptr, nullptr, &dst.w, &dst.h);
    }

    this->renderTexture(dst, clip);
}

void Texture::renderTexture(SDL_Rect &dst, SDL_Rect *clip)
{
    SDL_RenderCopy(renderer_->getRenderer(), texture_, clip, &dst);
}

void Texture::free()
{
    if (texture_)
        SDL_DestroyTexture(texture_);

    texture_ = nullptr;
}

Texture::Texture(Renderer* renderer)
{
    renderer_ = renderer;
}

int Texture::getWidth() const
{
    return width_;
}

int Texture::getHeight() const
{
    return height_;
}

int Texture::getX() const
{
    return x_;
}

int Texture::getY() const
{
    return y_;
}

void Texture::setX(int x)
{
    x_ = x;
}

void Texture::setY(int y)
{
    y_ = y;
}

void Texture::setWidth(int width)
{
    width_ = width;
}

void Texture::setHeight(int height)
{
    height_ = height;
}


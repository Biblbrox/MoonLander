#include <SDL_image.h>
#include "../include/texture.hpp"

void Texture::renderTexture(int x, int y, SDL_Rect *clip)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
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

bool Texture::loadFromFile(const std::string& path)
{
    this->free();

    this->texture = IMG_LoadTexture(this->renderer, path.c_str());

    if (this->texture == nullptr)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "LoadTexture");


    return this->texture != nullptr;
}

Texture::~Texture()
{
    if (this->texture)
        SDL_DestroyTexture(this->texture);
}

void Texture::free()
{
    if (this->texture)
        SDL_DestroyTexture(this->texture);

    this->texture = nullptr;
}

Texture::Texture(Renderer &renderer, const std::string& path)
{
    this->renderer = renderer.getRenderer();
    this->texture = IMG_LoadTexture(this->renderer, path.c_str());

    if (this->texture == nullptr)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "LoadTexture");
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



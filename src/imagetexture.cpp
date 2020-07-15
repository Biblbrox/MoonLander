#include <imagetexture.h>
#include <SDL_image.h>

ImageTexture::ImageTexture(Renderer& renderer, const std::string& path) :
        Texture(renderer)
{
    if (!path.empty()) {
        this->texture = IMG_LoadTexture(this->renderer, path.c_str());

        if (this->texture == nullptr)
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "LoadTexture");
    }
}




bool ImageTexture::load(const std::string& path)
{
    this->free();

    this->texture = IMG_LoadTexture(this->renderer, path.c_str());
    if (this->texture == nullptr)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "LoadTexture");


    return this->texture != nullptr;
}
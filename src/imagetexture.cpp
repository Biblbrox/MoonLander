#include <imagetexture.h>
#include <SDL_image.h>

ImageTexture::ImageTexture(Renderer* renderer, const std::string& path) :
        Texture(renderer)
{
    load(path);
    renderer_ = renderer;
}


bool ImageTexture::load(const std::string& path)
{
    if (!path.empty()) {
        this->free();

        texture_ = IMG_LoadTexture(renderer_->getRenderer(), path.c_str());
        if (texture_ == nullptr)
            SDL_Log("LoadTexture: %s\n", SDL_GetError());

    }

    return texture_ != nullptr;
}
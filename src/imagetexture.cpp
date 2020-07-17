#include <imagetexture.h>
#include <SDL_image.h>

ImageTexture::ImageTexture(const std::string& path)
{
    load(path);
}


bool ImageTexture::load(const std::string& path)
{
    if (!path.empty()) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (surface == nullptr) {
            SDL_Log("LoadTexture: %s\n", SDL_GetError());
            std::abort();
        }

        loadTextureFromPixels32(static_cast<GLuint*>(surface->pixels), surface->w, surface->h);
        SDL_FreeSurface(surface);
    }
}
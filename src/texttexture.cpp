#include <texttexture.h>
#include <string>
#include <SDL_ttf.h>

bool TextTexture::load(const std::string &textureText, SDL_Color color, TTF_Font& font)
{
    this->free();

    SDL_Surface* surface = TTF_RenderText_Solid(&font, textureText.c_str(), color);
    if (!surface) {
        SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        texture = SDL_CreateTextureFromSurface(this->renderer, surface);
        if (!texture) {
            SDL_Log("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        } else {
            this->height = surface->h;
            this->width = surface->w;
        }

        SDL_FreeSurface(surface);
    }

    return this->texture;
}

TextTexture::TextTexture(Renderer& renderer, const std::string &textureText,
        SDL_Color color, TTF_Font &font) : Texture(renderer)
{
    this->load(textureText, color, font);
}

#include <texttexture.h>
#include <string>
#include <SDL_ttf.h>

bool TextTexture::load(const std::string &textureText, SDL_Color color, TTF_Font* font)
{
    this->free();

    SDL_Surface* surface = TTF_RenderText_Solid(font, textureText.c_str(), color);
    if (!surface) {
        SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        texture_ = SDL_CreateTextureFromSurface(renderer_->getRenderer(), surface);
        if (!texture_) {
            SDL_Log("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        } else {
            height_ = surface->h;
            width_ = surface->w;
        }

        SDL_FreeSurface(surface);
    }

    return texture_;
}

TextTexture::TextTexture(Renderer* renderer, const std::string &textureText,
        SDL_Color color, TTF_Font *font) : Texture(renderer)
{
    this->load(textureText, color, font);
    font_ = font;
    color_ = color;
    text_ = textureText;
}

void TextTexture::setText(const std::string& text)
{
    load(text, color_, font_);
    text_ = text;
}

void TextTexture::setFont(TTF_Font* font)
{
    this->load(text_, color_, font);
    font_ = font;
}

void TextTexture::setColor(SDL_Color color)
{
    this->load(text_, color, font_);
    color_ = color;
}

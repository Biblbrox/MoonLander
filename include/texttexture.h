#ifndef MOONLANDER_TEXTTEXTURE_H
#define MOONLANDER_TEXTTEXTURE_H

#include <SDL_ttf.h>
#include "texture.hpp"

class TextTexture: public Texture {

public:
    TextTexture(Renderer* renderer, const std::string& textureText, SDL_Color color, TTF_Font* font);
    bool load(const std::string& textureText, SDL_Color color, TTF_Font* font);
    void setText(const std::string& text);
    void setFont(TTF_Font* font);
    void setColor(SDL_Color color);
private:
    std::string text_;
    TTF_Font* font_;
    SDL_Color color_;
};

#endif //MOONLANDER_TEXTTEXTURE_H

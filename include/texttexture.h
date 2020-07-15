#ifndef MOONLANDER_TEXTTEXTURE_H
#define MOONLANDER_TEXTTEXTURE_H

#include <SDL_ttf.h>
#include "texture.hpp"

class TextTexture: public Texture {

public:
    TextTexture(Renderer& renderer, const std::string& textureText, SDL_Color color, TTF_Font& font);
    bool load(const std::string& textureText, SDL_Color color, TTF_Font& font);
};

#endif //MOONLANDER_TEXTTEXTURE_H

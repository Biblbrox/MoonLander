#ifndef MOONLANDER_TEXTTEXTURE_HPP
#define MOONLANDER_TEXTTEXTURE_HPP

#include <SDL_ttf.h>
#include "texture.hpp"
#include "utils/utils.hpp"
#include "moonlanderprogram.hpp"

class TextTexture: public Texture
{

public:
    TextTexture(std::string textureText,
                SDL_Color color = {0x00, 0xFF, 0x00, 0xFF}, TTF_Font* font = nullptr);
    ~TextTexture() override;

    void load(const std::string& textureText, SDL_Color color, TTF_Font* font);
    void setText(const std::string& text);
    void setFont(TTF_Font* font);
    void setColor(SDL_Color color);

    void generateDataBuffer() override;
    void freeVBO() override final;

    GLuint getVAO() const override;
private:
    std::string m_text;
    TTF_Font* m_font;
    SDL_Color m_color;

    GLuint m_vaoId;
};

#endif //MOONLANDER_TEXTTEXTURE_HPP

#ifndef MOONLANDER_TEXTTEXTURE_HPP
#define MOONLANDER_TEXTTEXTURE_HPP

#include <SDL_ttf.h>
#include "texture.hpp"
#include "utils.hpp"
#include "moonlanderprogram.hpp"

class TextTexture: public Texture {

public:
    TextTexture(const std::string& textureText,
                SDL_Color color = {0x00, 0xFF, 0x00, 0xFF}, TTF_Font* font = nullptr);
    ~TextTexture() override;

    void load(const std::string& textureText, SDL_Color color, TTF_Font* font);
    void setText(const std::string& text);
    void setFont(TTF_Font* font);
    void setColor(SDL_Color color);
    void setScale(GLfloat scale_factor);

    void render(MoonLanderProgram& program, GLfloat x, GLfloat y, Utils::Rect* clip, GLfloat angle);
    void render(MoonLanderProgram& program, GLfloat x, GLfloat y, Utils::Rect* clip);

    void initVBO();
    void freeVBO();
    GLuint getVAO() const override;
private:
    std::string text;
    TTF_Font* font;
    SDL_Color color;

    GLuint vao_id;
    GLfloat scale_f;
};

#endif //MOONLANDER_TEXTTEXTURE_HPP

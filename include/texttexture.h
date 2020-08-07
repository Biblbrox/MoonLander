#ifndef MOONLANDER_TEXTTEXTURE_H
#define MOONLANDER_TEXTTEXTURE_H

#include <SDL_ttf.h>
#include "texture.hpp"
#include "utils.hpp"
#include "moonlanderprogram.h"

class TextTexture: public Texture {

public:
    TextTexture(const std::string& textureText, SDL_Color color, TTF_Font* font);
    ~TextTexture();

    bool load(const std::string& textureText, SDL_Color color, TTF_Font* font);
    void setText(const std::string& text);
    void setFont(TTF_Font* font);
    void setColor(SDL_Color color);

    void render(MoonLanderProgram& program, GLfloat x, GLfloat y, Utils::Rect* clip, double angle);
    void render(MoonLanderProgram& program, GLfloat x, GLfloat y, Utils::Rect* clip);

    void initVBO();
    void freeVBO();
    void freeTexture();
private:
    std::string text;
    TTF_Font* font;
    SDL_Color color;

    GLuint vbo_id;
    GLuint ibo_id;
    GLuint vao_id;
};

#endif //MOONLANDER_TEXTTEXTURE_H

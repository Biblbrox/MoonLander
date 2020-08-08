#ifndef MOONLANDER_SPRITE_H
#define MOONLANDER_SPRITE_H

#include "texture.hpp"
#include "utils.hpp"
#include "moonlanderprogram.h"

class Sprite: public Texture {
public:
    explicit Sprite(const std::string& path);
    ~Sprite();
    bool load(const std::string& path);
    GLuint addClipSprite(Utils::Rect clip);
    Utils::Rect getClip(int idx);
    bool generateDataBuffer();
    void render(MoonLanderProgram& program, GLfloat x, GLfloat y, GLuint idx, GLfloat angle);
    void freeSheet();
    void freeTexture();

protected:
    std::vector<Utils::Rect> clips;

    GLuint* VAO;
    GLuint tot_sprites;
};

#endif //MOONLANDER_SPRITE_H

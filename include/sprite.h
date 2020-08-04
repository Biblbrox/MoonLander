#ifndef MOONLANDER_SPRITE_H
#define MOONLANDER_SPRITE_H

#include "texture.hpp"
#include "utils.hpp"

class Sprite: public Texture {
public:
    explicit Sprite(const std::string& path);
    ~Sprite();
    bool load(const std::string& path);
    GLuint addClipSprite(Utils::Rect clip);
    Utils::Rect getClip(int idx);
    bool generateDataBuffer();
    void render(GLfloat x, GLfloat y, int idx, GLfloat angle);
    void freeSheet();
    void freeTexture();

protected:
    std::vector<Utils::Rect> clips;

    GLuint vertexDataBuffer;
    GLuint* indexBuffers;
};

#endif //MOONLANDER_SPRITE_H

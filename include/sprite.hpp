#ifndef MOONLANDER_SPRITE_HPP
#define MOONLANDER_SPRITE_HPP

#include "texture.hpp"
#include "utils.hpp"
#include "moonlanderprogram.hpp"

class Sprite: public Texture {
public:
    explicit Sprite(const std::string& path);
    ~Sprite();
    bool load(const std::string& path);
    GLuint addClipSprite(Utils::Rect clip);
    Utils::Rect getClip(int idx);

    GLuint getWidth() const override;
    GLuint getHeight() const override;
    Utils::Rect getCurrentClip() const;
    GLuint getIdx() const;
    void setIdx(GLuint idx);

    bool generateDataBuffer();
    //void render(MoonLanderProgram& program, GLfloat x, GLfloat y, GLuint idx, GLfloat angle);
    void freeSheet();

    GLuint getVAO() const;

protected:
    std::vector<Utils::Rect> clips;

    GLuint* VAO;
    GLuint tot_sprites;
    GLuint cur_idx;
};

#endif //MOONLANDER_SPRITE_HPP

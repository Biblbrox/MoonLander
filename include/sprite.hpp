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
    GLuint addClipSprite(utils::Rect clip);
    utils::Rect getClip(GLuint idx);

    GLuint getWidth() const override;
    GLuint getHeight() const override;
    utils::Rect getCurrentClip() const;
    GLuint getIdx() const;
    void setIdx(GLuint idx);

    void generateDataBuffer() override;
    void freeVBO() override final;

    GLuint getVAO() const override;
protected:
    std::vector<utils::Rect> clips;

    GLuint* VAO;
    GLuint tot_sprites;
    GLuint cur_idx = 0;
};

#endif //MOONLANDER_SPRITE_HPP

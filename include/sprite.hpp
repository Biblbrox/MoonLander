#ifndef MOONLANDER_SPRITE_HPP
#define MOONLANDER_SPRITE_HPP

#include "texture.hpp"
#include "utils/utils.hpp"
#include "moonlanderprogram.hpp"

/**
 * Sprite class.
 * Can contain one big texture with multiple small clips.
 */
class Sprite: public Texture
{
public:
    explicit Sprite(const std::string& path);
    ~Sprite();
    void load(const std::string& path);
    GLuint addClipSprite(utils::Rect clip);
    utils::Rect getClip(GLuint idx);

    GLuint getWidth() const override;
    GLuint getHeight() const override;
    utils::Rect getCurrentClip() const;
    GLuint getIdx() const;
    void setIdx(GLuint idx);
    GLuint getSpritesCount() const;

    void generateDataBuffer() override;
    void freeVBO() noexcept override final;

    GLuint getVAO() const override;
protected:
    std::vector<utils::Rect> m_clips;

    GLuint* m_vao;
    GLuint m_totSprites;
    GLuint m_curIdx = 0;
};

#endif //MOONLANDER_SPRITE_HPP

#include <GL/glew.h>
#include <sprite.hpp>
#include <SDL_image.h>
#include <exceptions/sdlexception.h>
#include <SDL_ttf.h>

using utils::log::Category;

Sprite::Sprite(const std::string& path)
{
    load(path);
    m_vao = nullptr;
    m_totSprites = 0;
    m_curIdx = 0;
}

void Sprite::load(const std::string& path)
{
    assert(!path.empty());
    freeTexture();
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface)
        throw SdlException((format("Unable to create blended text. "
                                   "SDL_ttf Error: %s\n")
                            % TTF_GetError()).str());
    SDL_Surface* flipped = utils::flipVertically(surface);
    if (!flipped)
        throw SdlException((format("Unable to flip surface %p\n") % surface).str());
    SDL_FreeSurface(surface);

    GLenum texture_format = utils::getSurfaceFormatInfo(*flipped);

    m_textureWidth = flipped->w;
    m_textureHeight = flipped->h;

    m_textureId = utils::loadTextureFromPixels32(
            static_cast<GLuint*>(flipped->pixels),
            m_textureWidth, m_textureHeight, texture_format);
    SDL_FreeSurface(flipped);
}

GLuint Sprite::addClipSprite(utils::Rect clip)
{
    m_clips.push_back(clip);
    return m_clips.size() - 1;
}

utils::Rect Sprite::getClip(GLuint idx)
{
    assert(idx < m_totSprites);
    return m_clips[idx];
}

void Sprite::generateDataBuffer()
{
    if (m_textureId != 0 && !m_clips.empty()) {
        m_totSprites = m_clips.size();
        m_vao = new GLuint[m_totSprites];

        glGenVertexArrays(m_totSprites, m_vao);
        GLuint VBO;
        GLuint EBO;
        GLfloat tw = m_textureWidth;
        GLfloat th = m_textureHeight;

        GLuint indices[] = {
                3, 1, 0,
                3, 2, 1
        };

        // 8 Vertex coordinates + 8 texture coordinates
        GLfloat vertices[16];

        vertices[1] = 0.f;
        vertices[8] = 0.f;
        vertices[12] = 0.f;
        vertices[13] = 0.f;

        for (GLuint i = 0; i < m_totSprites; ++i) {
            vertices[0] = m_clips[i].w;
            vertices[2] = (m_clips[i].x + m_clips[i].w) / tw;
            vertices[3] = (m_clips[i].y + m_clips[i].h) / th;
            vertices[4] = m_clips[i].w;
            vertices[5] = m_clips[i].h;
            vertices[6] = (m_clips[i].x + m_clips[i].w) / tw;
            vertices[7] = m_clips[i].y / th;
            vertices[9] = m_clips[i].h;
            vertices[10] = m_clips[i].x / tw;
            vertices[11] = m_clips[i].y / th;
            vertices[14] = m_clips[i].x / tw;
            vertices[15] = (m_clips[i].y + m_clips[i].h) / th;

            glBindVertexArray(m_vao[i]);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                         GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                         GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                                  4 * sizeof(GLfloat), nullptr);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                                  (void*)(2 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            glDeleteBuffers(1, &EBO);
            glDeleteBuffers(1, &VBO);
        }

    } else {
        if (m_textureId == 0) {
            Logger::write(utils::program_log_file_name(), Category::INTERNAL_ERROR,
                          "No texture to render\n");
            std::abort();
        }

        if (m_clips.empty()) {
            Logger::write(utils::program_log_file_name(), Category::INTERNAL_ERROR,
                          "No data generate from\n");
            std::abort();
        }
    }
}

void Sprite::freeVBO() noexcept
{
    if (m_vao) {
        glDeleteVertexArrays(m_totSprites, m_vao);
        delete[] m_vao;
        m_vao = nullptr;
    }

    m_clips.clear();
}

Sprite::~Sprite()
{
    freeVBO();
}

GLuint Sprite::getVAO() const
{
    return m_vao[m_curIdx];
}

GLuint Sprite::getIdx() const
{
    return m_curIdx;
}

utils::Rect Sprite::getCurrentClip() const
{
    return m_clips[m_curIdx];
}

void Sprite::setIdx(GLuint idx)
{
    assert(idx < this->m_totSprites);
    m_curIdx = idx;
}

GLuint Sprite::getWidth() const
{
    return getCurrentClip().w;
}

GLuint Sprite::getHeight() const
{
    return getCurrentClip().h;
}

GLuint Sprite::getSpritesCount() const
{
    return m_totSprites;
}


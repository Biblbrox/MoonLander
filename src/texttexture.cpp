#include <GL/glew.h>
#include <texttexture.hpp>
#include <string>
#include <SDL_ttf.h>
#include <exceptions/sdlexception.h>

using utils::getResourcePath;
using boost::format;

void TextTexture::setText(const std::string& text)
{
    load(text, m_color, m_font);
    m_text = text;
}

void TextTexture::setFont(TTF_Font* font)
{
    load(m_text, m_color, font);
    m_font = font;
}

void TextTexture::setColor(SDL_Color color)
{
    load(m_text, color, m_font);
    m_color = color;
}

TextTexture::TextTexture(std::string textureText, SDL_Color color,
                         TTF_Font *font)
{
    if (!font) {
        m_font = TTF_OpenFont(getResourcePath("kenvector_future2.ttf").c_str(), 14);
        if (!m_font) {
            SDL_Log("TTF_OpenFont error: %s\n", TTF_GetError());
            std::abort();
        }
    } else {
        m_font = font;
    }

    m_color = color;
    m_text = textureText;
    m_vaoId = 0;
    m_textureId = 0;
    load(textureText, m_color, m_font);
}

void TextTexture::load(const std::string &textureText, SDL_Color color,
                       TTF_Font* font)
{
    freeTexture();
    SDL_Surface* surface = TTF_RenderText_Blended(font,
                                                  textureText.c_str(), color);

    if (!surface)
        throw SdlException((format("Unable to create blended text. "
                                   "SDL_ttf Error: %s\n")
                            % TTF_GetError()).str());

    SDL_Surface* flipped = utils::flipVertically(surface);
    SDL_FreeSurface(surface);
    if (!flipped) {
        throw SdlException((format("Unable to flip surface %p\n") % surface).str());
    } else {
        GLenum texture_format = utils::getSurfaceFormatInfo(*flipped);

        m_textureWidth = flipped->w;
        m_textureHeight = flipped->h;

        m_textureId = utils::loadTextureFromPixels32
                (static_cast<GLuint*>(flipped->pixels),
                 m_textureWidth, m_textureHeight, texture_format);
        SDL_FreeSurface(flipped);

        generateDataBuffer();
    }
}

void TextTexture::generateDataBuffer()
{
    if (m_textureId != 0 && m_vaoId == 0) {
        GLfloat quadWidth = m_textureWidth;
        GLfloat quadHeight = m_textureHeight;

        GLfloat vertices[] = {
                // positions            // texture coords
                quadWidth,  0.0f,       1.0f, 1.0f, // top right
                quadWidth, quadHeight,  1.0f, 0.0f, // bottom right
                0.f, quadHeight,        0.0f, 0.0f, // bottom left
                0.f,  0.f,              0.0f, 1.0f  // top left
        };

        GLuint indices[] = {
                3, 1, 0,
                3, 2, 1
        };

        GLuint vbo_id;
        GLuint ibo_id;

        glGenVertexArrays(1, &m_vaoId);
        glGenBuffers(1, &vbo_id);
        glGenBuffers(1, &ibo_id);

        glBindVertexArray(m_vaoId);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                     GL_STATIC_DRAW);

        //position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                              4 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0);

        //Tex coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                              4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDeleteBuffers(1, &vbo_id);
        glDeleteBuffers(1, &ibo_id);
    }
}

void TextTexture::freeVBO()
{
    if (m_vaoId != 0)
        glDeleteVertexArrays(1, &m_vaoId);
}

TextTexture::~TextTexture()
{
    if (m_font)
        TTF_CloseFont(m_font);
    freeVBO();
}

GLuint TextTexture::getVAO() const
{
    return m_vaoId;
}

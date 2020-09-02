#include <GL/glew.h>
#include <texttexture.hpp>
#include <string>
#include <SDL_ttf.h>

void TextTexture::setText(const std::string& text_)
{
    load(text_, color, font);
    text = text_;
}

void TextTexture::setFont(TTF_Font* font_)
{
    load(text, color, font_);
    font = font_;
}

void TextTexture::setColor(SDL_Color color_)
{
    load(text, color_, font);
    color = color_;
}

TextTexture::TextTexture(std::string textureText, SDL_Color color_, TTF_Font *font_)
{
    if (font_ == nullptr) {
        font = TTF_OpenFont(
                utils::getResourcePath("kenvector_future2.ttf").c_str(), 14);
        if (font == NULL) {
            SDL_Log("TTF_OpenFont error: %s\n", TTF_GetError());
            std::abort();
        }
    } else {
        font = font_;
    }

    color = color_;
    text = textureText;
    vao_id = 0;
    textureID = 0;
    load(textureText, color, font);
}

void TextTexture::load(const std::string &textureText, SDL_Color color_, TTF_Font* font_)
{
    freeTexture();
    SDL_Surface* surface = TTF_RenderText_Blended(font_, textureText.c_str(), color_);

    surface = utils::flipVertically(surface);
    if (!surface) {
        SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        GLenum texture_format = utils::getSurfaceFormatInfo(*surface);

        texture_width = surface->w;
        texture_height = surface->h;

        textureID = utils::loadTextureFromPixels32(static_cast<GLuint*>(surface->pixels),
                                                   texture_width, texture_height, texture_format);
        SDL_FreeSurface(surface);

        generateDataBuffer();
    }
}

void TextTexture::generateDataBuffer()
{
    if (textureID != 0 && vao_id == 0) {
        GLfloat quadWidth = texture_width;
        GLfloat quadHeight = texture_height;

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

        glGenVertexArrays(1, &vao_id);
        glGenBuffers(1, &vbo_id);
        glGenBuffers(1, &ibo_id);

        glBindVertexArray(vao_id);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

        //position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0);

        //Tex coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
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
    if (vao_id != 0)
        glDeleteVertexArrays(1, &vao_id);
}

TextTexture::~TextTexture()
{
    TTF_CloseFont(font);
    freeVBO();
}

GLuint TextTexture::getVAO() const
{
    return vao_id;
}

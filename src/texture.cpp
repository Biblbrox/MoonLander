#include <SDL_image.h>
#include <SDL_ttf.h>
#include <GL/glu.h>
#include "../include/texture.hpp"

Texture::~Texture()
{
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    texture_height = texture_width = 0;
}

void Texture::renderTexture(int x, int y, Utils::Rect *clip)
{

//    SDL_Rect dst;
//    dst.x = x;
//    dst.y = y;
//    if (clip != nullptr) {
//        dst.w = clip->w;
//        dst.h = clip->h;
//    } else {
//        SDL_QueryTexture(texture_, nullptr, nullptr, &dst.w, &dst.h);
//    }
//
//    this->renderTexture(dst, clip);
}

void Texture::renderTexture(int x, int y, SDL_Rect* clip, double angle,
        SDL_Point* center, SDL_RendererFlip flip)
{
//    SDL_Rect renderQuad = {x, y, texture_width, texture_height};
//
//    if (clip != nullptr) {
//        renderQuad.w = clip->w;
//        renderQuad.h = clip->h;
//    }
//
//    SDL_RenderCopyEx(renderer_->getRenderer(), texture_, clip, &renderQuad, angle, center, flip);
}

void Texture::renderTexture(SDL_Rect &dst, SDL_Rect *clip)
{
//    SDL_RenderCopy(renderer_->getRenderer(), texture_, clip, &dst);
}

void Texture::free()
{
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    texture_width = texture_height = 0;
}

Texture::Texture()
{
    textureID = 0;

    texture_width = 0;
    texture_height = 0;
}

GLuint Texture::getWidth() const
{
    return texture_width;
}

GLuint Texture::getHeight() const
{
    return texture_height;
}

int Texture::getX() const
{
    return x_;
}

int Texture::getY() const
{
    return y_;
}

void Texture::setX(int x)
{
    x_ = x;
}

void Texture::setY(int y)
{
    y_ = y;
}

void Texture::setWidth(int width)
{
    texture_width = width;
}

void Texture::setHeight(int height)
{
    texture_height = height;
}

void Texture::loadTextureFromPixels32(GLuint *pixels, GLuint width, GLuint height)
{
    free();

    texture_width = width;
    texture_height = height;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    glBindTexture(GL_TEXTURE_2D, 0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        SDL_Log("Error loading texture from %p pixels! %s\n", pixels, gluErrorString(error));
        std::abort();
    }
}

void Texture::render(GLfloat x, GLfloat y, Utils::Rect* clip)
{
    if (textureID != 0) {
        //glLoadIdentity();

        GLfloat texTop = 0.f;
        GLfloat texBottom = 1.f;
        GLfloat texRight = 1.f;
        GLfloat texLeft = 0.f;

        GLfloat quadWidth = texture_width;
        GLfloat quadHeight = texture_height;

        if (clip != nullptr) {
            texLeft = clip->x / texture_width;
            texRight = (clip->x + clip->w) / texture_width;
            texTop = clip->y / texture_height;
            texBottom = (clip->y + clip->h) / texture_height;

            quadWidth = clip->w;
            quadHeight = clip->h;
        }


        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTranslatef(x, y, 0.f);

        glBegin(GL_QUADS);
        glTexCoord2f(texLeft, texTop); glVertex2f(0.f, 0.f);
        glTexCoord2f(texRight, texTop); glVertex2f(quadWidth, 0.f);
        glTexCoord2f(texRight, texBottom); glVertex2f(quadWidth, quadHeight);
        glTexCoord2f(texLeft, texBottom); glVertex2f(0.f, quadHeight);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);
        glPopMatrix();
    }
}

GLuint Texture::getTextureID()
{
    return textureID;
}


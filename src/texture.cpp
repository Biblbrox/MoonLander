#include <SDL_image.h>
#include <SDL_ttf.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "../include/texture.hpp"

Texture::~Texture()
{
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    texture_height = texture_width = 0;

    if (vbo_id != 0) {
        glDeleteBuffers(1, &vbo_id);
        glDeleteBuffers(1, &ibo_id);
    }
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

    vbo_id = ibo_id = 0;
}

GLuint Texture::getWidth() const
{
    return texture_width;
}

GLuint Texture::getHeight() const
{
    return texture_height;
}

GLfloat Texture::getX() const
{
    return x_;
}

GLfloat Texture::getY() const
{
    return y_;
}

void Texture::setX(GLfloat x)
{
    x_ = x;
}

void Texture::setY(GLfloat y)
{
    y_ = y;
}

void Texture::setWidth(GLuint width)
{
    texture_width = width;
}

void Texture::setHeight(GLuint height)
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

    initVBO();
}

void Texture::render(GLfloat x, GLfloat y, Utils::Rect* clip, double angle)
{
    GLuint clip_w = texture_width;
    GLuint clip_h = texture_height;
    if (clip != nullptr) {
        clip_w = clip->w;
        clip_h = clip->h;
    }

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTranslatef(getX() + clip_w / 2.f, getY() + clip_h / 2.f, 0);
    glRotatef(angle, 0.f, 0.f, 1.f);
    glTranslatef(-(getX() + clip_w / 2.f), -(getY() + clip_h / 2.f), 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    render(x, y, clip);
    glPopMatrix();
}

void Texture::render(GLfloat x, GLfloat y, Utils::Rect* clip)
{
    if (textureID != 0) {
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

        Utils::VertexData2D vData[4];
        vData[0].texCoord.x = texLeft; vData[0].texCoord.y = texTop;
        vData[1].texCoord.x = texRight; vData[1].texCoord.y = texTop;
        vData[2].texCoord.x = texRight; vData[2].texCoord.y = texBottom;
        vData[3].texCoord.x = texLeft; vData[3].texCoord.y = texBottom;

        vData[0].position.x = 0.f; vData[0].position.y = 0.f;
        vData[1].position.x = quadWidth; vData[1].position.y = 0.f;
        vData[2].position.x = quadWidth; vData[2].position.y = quadHeight;
        vData[3].position.x = 0.f; vData[3].position.y = quadHeight;

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(Utils::VertexData2D), vData);
        glTexCoordPointer(2, GL_FLOAT, sizeof(Utils::VertexData2D), (GLvoid*)(offsetof(Utils::VertexData2D, texCoord)));
        glVertexPointer(2, GL_FLOAT, sizeof(Utils::VertexData2D), (GLvoid*)(offsetof(Utils::VertexData2D, position)));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
        glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, NULL);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        glBindTexture(GL_TEXTURE_2D, 0);
        glPopMatrix();
    }
}

GLuint Texture::getTextureID()
{
    return textureID;
}

void Texture::initVBO()
{
    if (textureID != 0 && vbo_id == 0) {
        Utils::VertexData2D vData[4];
        GLuint iData[4];

        iData[0] = 0;
        iData[1] = 1;
        iData[2] = 2;
        iData[3] = 3;

        glGenBuffers(1, &vbo_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Utils::VertexData2D), vData, GL_DYNAMIC_DRAW);

        glGenBuffers(1, &ibo_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), iData, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, NULL);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
    }
}

void Texture::freeVBO()
{
    if (vbo_id != 0) {
        glDeleteBuffers(1, &vbo_id);
        glDeleteBuffers(1, &ibo_id);
    }
}


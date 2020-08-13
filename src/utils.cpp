#include <ctime>
#include "../include/utils.hpp"

Utils::RandomUniform::RandomUniform() : generator(std::time(nullptr))
{

}

GLuint Utils::loadTextureFromPixels32(GLuint *pixels, GLuint width, GLuint height, GLenum format)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format,
                 GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("Error loading texture from %p pixels! %s\n", pixels, gluErrorString(error));
        std::abort();
    }

    return textureID;
}


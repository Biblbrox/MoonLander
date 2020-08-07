#include <ctime>
#include "../include/utils.hpp"

int Utils::RandomUniform::generate(int a, int b)
{
    std::uniform_int_distribution<int> dist(a, b);
    return dist(this->generator);
}

Utils::RandomUniform::RandomUniform() : generator(std::time(nullptr))
{

}

GLuint Utils::loadTextureFromPixels32(GLuint *pixels, GLuint width, GLuint height, GLint noColors, GLenum format)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, noColors, width, height, 0, format,
                 GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("Error loading texture from %p pixels! %s\n", pixels, gluErrorString(error));
        std::abort();
    }

    return textureID;
}


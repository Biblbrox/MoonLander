#include <ctime>
#include "../include/utils.hpp"
#include <boost/format.hpp>

utils::RandomUniform::RandomUniform() : generator(std::time(nullptr))
{

}

/**
 * Load opengl texture from pixels with specific format.
 * Result texture has RGBA format.
 * If function can't load texture exception will be thrown.
 * @param pixels
 * @param width
 * @param height
 * @param format
 * @return
 */
GLuint utils::loadTextureFromPixels32(GLuint *pixels, GLuint width, GLuint height, GLenum format)
{
    assert(pixels != nullptr);
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
    if (error != GL_NO_ERROR)
        throw std::runtime_error((boost::format("Error loading texture from %1% pixels! %2%\n")
                                  % pixels % gluErrorString(error)).str());

    return textureID;
}

/**
 * Write msg to the standard output and log file
 * @param userdata
 * @param category
 * @param priority
 * @param msg
 */
void utils::log::log_function(void* userdata, int category, SDL_LogPriority priority, const char* msg)
{
    std::cout << "Hello from log function" << std::endl;
}

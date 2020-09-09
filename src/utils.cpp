#include <ctime>
#include "../include/utils.hpp"
#include <boost/format.hpp>

using utils::log::Logger;
using glm::vec2;

utils::RandomUniform::RandomUniform() : generator(std::time(nullptr))
{

}

/**
 * Load opengl texture from pixels to GPU with specific format.
 * Result texture has RGBA format.
 * If function can't load texture exception will be thrown.
 * @param pixels
 * @param width
 * @param height
 * @param format
 * @return textureID
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
    if (error != GL_NO_ERROR) {
        Logger::write(program_log_file_name(),
                      log::Category::INTERNAL_ERROR,
                      (boost::format(
                              "Error loading texture from %1% pixels! %2%\n")
                       % pixels % gluErrorString(error)).str());
        std::abort();
    }

    return textureID;
}

GLfloat utils::physics::ship_altitude(const std::vector<vec2>& line_points, GLfloat shipX, GLfloat shipY)
{
    assert(line_points.size() >= 2 && "line_points size must be >= 2");
    size_t line_idx = 0;
    for (size_t i = 0; i < line_points.size() - 1; ++i) {
        if (line_points[i].x <= shipX && line_points[i + 1].x > shipX) {
            line_idx = i;
            break;
        }
    }

    GLfloat alt = (shipX - line_points[line_idx].x)
                  / (line_points[line_idx + 1].x - line_points[line_idx].x)
                  * (line_points[line_idx + 1].y - line_points[line_idx].y)
                  + line_points[line_idx].y - shipY;

    return alt;
}

GLfloat utils::physics::alt_from_surface(const std::vector<vec2>& line_points, const GLfloat x, const GLfloat alt)
{
    assert(line_points.size() >= 2 && "line_points size must be >= 2");
    size_t line_idx;
    for (size_t i = 0; i < line_points.size() - 1; ++i) {
        if (line_points[i].x <= x && line_points[i + 1].x > x) {
            line_idx = i;
            break;
        }
    }

    GLfloat res = (x - line_points[line_idx].x)
                  / (line_points[line_idx + 1].x - line_points[line_idx].x)
                  * (line_points[line_idx + 1].y - line_points[line_idx].y)
                  + line_points[line_idx].y - alt;

    return res;
}
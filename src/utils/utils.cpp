#include <ctime>
#include "utils/utils.hpp"
#include <boost/format.hpp>

using utils::log::Logger;
using boost::format;
using glm::vec2;
using std::find_if;

/**
 * Load opengl texture from pixels to GPU with specific format.
 * Result texture has RGBA format.
 * If function can't load texture exception will be thrown.
 * @param pixels
 * @param width
 * @param height
 * @param texture_format
 * @return textureID
 */
GLuint
utils::loadTextureFromPixels32(const GLuint *pixels, GLuint width, GLuint height,
                               GLenum texture_format)
{
    assert(pixels != nullptr);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, texture_format,
                 GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (GLuint error = glGetError(); error != GL_NO_ERROR) {
        Logger::write(program_log_file_name(), log::Category::INTERNAL_ERROR,
                      (format("Error loading texture from %1% pixels! %2%\n")
                       % pixels % gluErrorString(error)).str());
        std::abort();
    }

    return textureID;
}

/**
 * Return altitude of position (x, y) respect to corresponding
 * line from line_points.
 * @param line_points
 * @param x
 * @param y
 * @return
 */
GLfloat
utils::physics::altitude(const std::vector<vec2>& line_points,
                              GLfloat x, GLfloat y)
{
    assert(line_points.size() >= 2 && "line_points size must be >= 2");
    size_t line_idx = find_if(line_points.cbegin(), line_points.cend(),
                              [x](const vec2& point){ return point.x > x; })
                      - line_points.cbegin();

    const GLfloat curX = line_points[line_idx].x;
    const GLfloat nextX = line_points[line_idx + 1].x;
    const GLfloat curY = line_points[line_idx].y;
    const GLfloat nextY = line_points[line_idx + 1].y;

    return (x - curX) / (nextX - curX) * (nextY - curY) + curY - y;
}

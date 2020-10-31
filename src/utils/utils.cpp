#include <ctime>
#include "utils/utils.hpp"
#include <boost/format.hpp>
#include <iostream>
#include <fstream>

using utils::log::Logger;
using utils::log::Category;
using utils::log::program_log_file_name;
using utils::log::shader_log_file_name;
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

    if (GLuint error = glGetError(); error != GL_NO_ERROR)
        throw GLException((format("Error loading texture from %1% pixels! %2%\n")
                           % pixels % gluErrorString(error)).str(),
                          program_log_file_name(), Category::INTERNAL_ERROR);

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

GLfloat utils::physics::coord_of_alt(const std::vector<vec2>& line_points,
                                      GLfloat x, GLfloat height)
{
    size_t line_idx = find_if(line_points.cbegin(), line_points.cend(),
                              [x](const vec2& point){ return point.x > x; })
                      - line_points.cbegin();
    return (line_points[line_idx].y + line_points[line_idx + 1].y) / 2.f - height;
}

GLuint utils::loadShaderFromFile(const std::string &path, GLenum shaderType)
{
    assert(!path.empty() && "Empty file path");
    GLuint shaderID = 0;
    std::string shaderString;
    std::ifstream sourceFile(path.c_str());
    if (!sourceFile.is_open())
        throw FSException((format("Can't open shader source file %1%\n")
                           % path).str(), program_log_file_name(),
                          utils::log::Category::FILE_ERROR);

    shaderString.assign(std::istreambuf_iterator<char>(sourceFile),
                        std::istreambuf_iterator<char>());

    shaderID = glCreateShader(shaderType);
    const GLchar *shaderSource = shaderString.c_str();
    glShaderSource(shaderID, 1, (const GLchar**) &shaderSource, NULL);
    glCompileShader(shaderID);

    GLint shaderCompiled = GL_FALSE;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
    if (shaderCompiled != GL_TRUE) {
        glDeleteShader(shaderID);
        sourceFile.close();
        utils::log::printShaderLog(shaderID);
        throw GLException(
                "Error while compiling shader(see shader log)!",
                program_log_file_name(),
                utils::log::Category::SHADER_COMPILE_ERROR);
    }

    sourceFile.close();
    return shaderID;
}
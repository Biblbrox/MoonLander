#include <GL/glew.h>
#include <abstract/shaderprogram.hpp>
#include <utils/utils.hpp>

using boost::format;
using utils::log::Logger;
using utils::log::Category;
using utils::log::program_log_file_name;
using utils::log::shader_log_file_name;

ShaderProgram::ShaderProgram()
{
    m_programID = 0;
}

ShaderProgram::~ShaderProgram()
{
    freeProgram();
}

void ShaderProgram::freeProgram()
{
    glDeleteProgram(m_programID);
}

void ShaderProgram::bind() const
{
    glUseProgram(m_programID);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_programID);
        throw GLException((format("Unable to bind shader program! %s%\n") %
                           gluErrorString(error)).str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }
}

void ShaderProgram::unbind()
{
    glUseProgram(0);
}

GLuint ShaderProgram::getProgramID()
{
    return m_programID;
}

void ShaderProgram::setInt(const std::string& name, GLint value)
{
    using utils::log::Logger;

    assert(!name.empty());
    GLint loc = glGetUniformLocation(m_programID, name.c_str());
    if (loc == -1)
        throw GLException((format("Unable to set uniform variable %1%\n") %
                           name).str(),
                          shader_log_file_name(),
                          Category::INTERNAL_ERROR);

    glUniform1i(loc, value);
    if (GLenum error = glGetError(); error != GL_NO_ERROR)
        throw GLException((format("Unable to set uniform variable \"%1%\"\n") % name).str(),
                          shader_log_file_name(),
                          Category::INTERNAL_ERROR);
}

void ShaderProgram::setFloat(const std::string &name, GLfloat value)
{
    using utils::log::Logger;

    assert(!name.empty());
    GLint loc = glGetUniformLocation(m_programID, name.c_str());
    if (loc == -1)
        throw GLException(
                (format("Can't find location by name \"%1%\"\n") % name).str(),
                shader_log_file_name(),
                Category::INTERNAL_ERROR);

    glUniform1f(loc, value);
    if (GLenum error = glGetError(); error != GL_NO_ERROR)
        throw GLException(
                (format("Unable to set uniform variable \"%1%\"\n") % name).str(),
                shader_log_file_name(),
                Category::INTERNAL_ERROR);
}

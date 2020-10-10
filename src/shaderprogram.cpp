#include <GL/glew.h>
#include <abstract/shaderprogram.hpp>
#include <cstdio>
#include <utils/utils.hpp>

using boost::format;
using utils::log::Logger;
using utils::log::Category;

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
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        Logger::write(utils::program_log_file_name(),
                      utils::log::Category::INTERNAL_ERROR,
                      (format("Erro binding shader program! %s%\n") %
                       gluErrorString(error)).str());
        utils::log::printProgramLog(m_programID);
        std::abort();
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
    if (loc == -1) {
        Logger::write(utils::shader_log_file_name(),
                      utils::log::Category::INTERNAL_ERROR,
                      (format("Unable to set uniform variable %1%\n") %
                       name).str());
        std::abort();
    }
    glUniform1i(loc, value);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        Logger::write(utils::program_log_file_name(),Category::INTERNAL_ERROR,
                      (format("Unable to set uniform variable \"%1%\"\n") % name).str());
        std::abort();
    }
}

void ShaderProgram::setFloat(const std::string &name, GLfloat value)
{
    using utils::log::Logger;

    assert(!name.empty());
    GLint loc = glGetUniformLocation(m_programID, name.c_str());
    if (loc == -1) {
        Logger::write(utils::program_log_file_name(),Category::INTERNAL_ERROR,
                      (format("Can't find location by name \"%1%\"\n") % name).str());
        std::abort();
    }
    glUniform1f(loc, value);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        Logger::write(utils::shader_log_file_name(),Category::INTERNAL_ERROR,
                      (format("Unable to set uniform variable \"%1%\"\n") % name).str());
        std::abort();
    }
}

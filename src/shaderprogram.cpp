#include <GL/glew.h>
#include <abstract/shaderprogram.hpp>
#include <cstdio>
#include <utils.hpp>

using boost::format;

ShaderProgram::ShaderProgram()
{
    programID = 0;
}

ShaderProgram::~ShaderProgram()
{
    freeProgram();
}

void ShaderProgram::freeProgram()
{
    glDeleteProgram(programID);
}

bool ShaderProgram::bind() const
{
    glUseProgram(programID);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("Error binding shader program! %s\n", gluErrorString(error));
        utils::log::printProgramLog(programID);
        std::abort();
        return false;
    }

    return true;
}

void ShaderProgram::unbind()
{
    glUseProgram(0);
}

GLuint ShaderProgram::getProgramID()
{
    return programID;
}

void ShaderProgram::setInt(const std::string& name, GLint value)
{
    assert(!name.empty());
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
    utils::log::Logger::write(utils::shader_log_file_name(),
                              utils::log::Category::INTERNAL_ERROR,
                              (format("Unable to set uniform variable %1%\n") % name).str());
    std::abort();
}

void ShaderProgram::setFloat(const std::string &name, GLfloat value)
{
    using utils::log::Logger;

    assert(!name.empty());
    GLint loc = glGetUniformLocation(programID, name.c_str());
    if (loc == -1) {
        utils::log::Logger::write(utils::shader_log_file_name(),
                                  utils::log::Category::INTERNAL_ERROR,
                                  (format("Can't find location by name \"%1%\"\n") % name).str());
        std::abort();
    }
    glUniform1f(loc, value);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        Logger::write(utils::shader_log_file_name(),
                      utils::log::Category::INTERNAL_ERROR,
                      (format("Unable to set uniform variable \"%1%\"\n") % name).str());
        std::abort();
    }
}

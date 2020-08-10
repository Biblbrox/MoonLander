#include <GL/glew.h>
#include <shaderprogram.h>
#include <cstdio>
#include <utils.hpp>

ShaderProgram::ShaderProgram()
{
    programID = 0;
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(programID);
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
        Utils::printProgramLog(programID);
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

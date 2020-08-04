#include <GL/glew.h>
#include <utils.hpp>
#include "../include/moonlanderprogram.h"

MoonLanderProgram::MoonLanderProgram()
{
    modelLoc = 0;
    viewLoc = 0;
    projectionLoc = 0;
}

bool MoonLanderProgram::loadProgram()
{
    programID = glCreateProgram();
    GLuint vertexShader = Utils::loadShaderFromFile(
            Utils::getShaderPath("moonLander.glvs"), GL_VERTEX_SHADER);

    if (vertexShader == 0) {
        glDeleteProgram( programID );
        programID = 0;
        return false;
    }

    glAttachShader(programID, vertexShader);

    GLuint fragmentShader = Utils::loadShaderFromFile(
            Utils::getShaderPath("moonLander.glfs"), GL_FRAGMENT_SHADER);

    if (fragmentShader == 0) {
        glDeleteProgram(programID);
        programID = 0;
        return false;
    }

    glAttachShader(programID, fragmentShader);

    glLinkProgram(programID);
    GLint programSuccess = GL_TRUE;
    glGetProgramiv(programID, GL_LINK_STATUS, &programSuccess);
    if (programSuccess != GL_TRUE) {
        printf( "Error linking program %d!\n", programID );
        Utils::printProgramLog( programID );
        glDeleteShader( vertexShader );
        glDeleteShader( fragmentShader );
        glDeleteProgram( programID );
        programID = 0;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    projectionLoc = glGetUniformLocation(programID, "ProjectionMatrix");
    if(projectionLoc == -1)
    {
        printf( "%s is not a valid glsl program variable!\n", "ProjectionMatrix" );
    }

    modelLoc = glGetUniformLocation(programID, "ModelMatrix");
    if (modelLoc == 1) {
        printf( "%s is not a valid glsl program variable!\n", "ModelMatrix" );
    }

    viewLoc = glGetUniformLocation(programID, "ViewMatrix");
    if (modelLoc == 1) {
        printf( "%s is not a valid glsl program variable!\n", "ViewMatrix" );
    }

    return true;
}

void MoonLanderProgram::setProjection(glm::mat4 matrix)
{
    projectionMatrix = matrix;
}

void MoonLanderProgram::setView(glm::mat4 matrix)
{
    viewMatrix = matrix;
}

void MoonLanderProgram::setModel(glm::mat4 matrix)
{
    modelMatrix = matrix;
}

void MoonLanderProgram::leftMultModelView(glm::mat4 matrix)
{
    modelMatrix = matrix * modelMatrix;
}

void MoonLanderProgram::leftMultProjection(glm::mat4 matrix)
{
    projectionMatrix = matrix * projectionMatrix;
}

void MoonLanderProgram::updateProjection()
{
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void MoonLanderProgram::updateModel()
{
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void MoonLanderProgram::updateView()
{
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}
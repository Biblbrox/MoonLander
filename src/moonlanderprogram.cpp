#include <GL/glew.h>
#include <utils.hpp>
#include "../include/moonlanderprogram.hpp"

MoonLanderProgram::MoonLanderProgram()
{
    modelLoc = 0;
    viewLoc = 0;
    projectionLoc = 0;
    texLoc = 0;
}

bool MoonLanderProgram::loadProgram()
{
    programID = glCreateProgram();
    // Vertex shader
    GLuint vertexShader = utils::loadShaderFromFile(
            utils::getShaderPath("moonLander.glvs"), GL_VERTEX_SHADER);

    if (vertexShader == 0) {
        glDeleteProgram(programID);
        programID = 0;
        return false;
    }

    glAttachShader(programID, vertexShader);
    // Vertex shader end

    // Geometry shader
//    GLuint geometryShader = utils::loadShaderFromFile(
//            utils::getShaderPath("moonLander.glgs"), GL_GEOMETRY_SHADER);
//
//    if (geometryShader == 0) {
//        glDeleteProgram(programID);
//        programID = 0;
//        return false;
//    }
//
//    glAttachShader(programID, geometryShader);
    // Geometry shader end

    // Fragment shader
    GLuint fragmentShader = utils::loadShaderFromFile(
            utils::getShaderPath("moonLander.glfs"), GL_FRAGMENT_SHADER);

    if (fragmentShader == 0) {
        glDeleteProgram(programID);
        programID = 0;
        return false;
    }

    glAttachShader(programID, fragmentShader);
    // Fragment shader end

    glLinkProgram(programID);
    GLint programSuccess = GL_TRUE;
    glGetProgramiv(programID, GL_LINK_STATUS, &programSuccess);
    if (programSuccess != GL_TRUE) {
        printf("Error linking program %d!\n", programID);
        utils::log::printProgramLog(programID);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        //glDeleteShader(geometryShader);
        glDeleteProgram(programID);
        programID = 0;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //glDeleteShader(geometryShader);

    projectionLoc = glGetUniformLocation(programID, "ProjectionMatrix");
    if(projectionLoc == -1)
    {
        printf( "%s is not a valid glsl program variable!\n", "ProjectionMatrix" );
    }

    modelLoc = glGetUniformLocation(programID, "ModelMatrix");
    if (modelLoc == -1) {
        printf( "%s is not a valid glsl program variable!\n", "ModelMatrix" );
    }

    viewLoc = glGetUniformLocation(programID, "ViewMatrix");
    if (modelLoc == -1) {
        printf( "%s is not a valid glsl program variable!\n", "ViewMatrix" );
    }

    colorLoc = glGetUniformLocation(programID, "inColor");
    if (colorLoc == -1) {
        printf("%s is not a valid glsl program variable!\n", "inColor");
    }

    isTextureLoc = glGetUniformLocation(programID, "renderTexture");
    if (isTextureLoc == -1) {
        printf( "%s is not a valid glsl program variable!\n", "renderTexture" );
    }

    texLoc = glGetUniformLocation(programID, "ourTexture");
    if (texLoc == -1) {
        printf( "%s is not a valid glsl program variable!\n", "ourTexture" );
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

void MoonLanderProgram::leftMultModel(glm::mat4 matrix)
{
    modelMatrix = matrix * modelMatrix;
}

void MoonLanderProgram::leftMultView(glm::mat4 matrix)
{
    viewMatrix = matrix * viewMatrix;
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

void MoonLanderProgram::setColor(glm::vec4 color)
{
    this->color = color;
}

void MoonLanderProgram::updateColor()
{
    glUniform4f(colorLoc, color.x, color.y, color.z, color.w);
}

void MoonLanderProgram::setTextureRendering(bool isTexture)
{
    glUniform1i(isTextureLoc, isTexture);
}

void MoonLanderProgram::setTexture(int texture_)
{
    glUniform1i(texLoc, texture_);
}

glm::mat4 MoonLanderProgram::getView() const
{
    return viewMatrix;
}

glm::mat4 MoonLanderProgram::getProjection() const
{
    return projectionMatrix;
}

glm::mat4 MoonLanderProgram::getModel() const
{
    return modelMatrix;
}

MoonLanderProgram::~MoonLanderProgram() = default;

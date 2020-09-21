#include <GL/glew.h>
#include <utils.hpp>
#include "../include/moonlanderprogram.hpp"

using utils::getShaderPath;
using utils::loadShaderFromFile;
using glm::mat4;
using glm::vec3;
using glm::vec4;

const int gl_bool_size = 4;
const int gl_int_size = 4;
const int gl_float_size = 4;

constexpr int next_offset(int cur_offset, int base_alignment)
{
    if (cur_offset % base_alignment == 0)
        return cur_offset;

    return cur_offset + base_alignment - cur_offset % base_alignment;
}

MoonLanderProgram::MoonLanderProgram()
{
    isTextureRender = false;
    texLoc = 0;
    cur_program = 0;
    colorLoc = 0;
}

bool MoonLanderProgram::loadProgram()
{
    for (int i = 0; i < std::size(programs); ++i) {
        programs[i] = glCreateProgram();
        if (programs[i] == 0) {
            std::abort();
        }
    }

    // Vertex shader
    GLuint vertexShader = loadShaderFromFile(
            getShaderPath("moonLander.glvs"), GL_VERTEX_SHADER);

    if (vertexShader == 0) {
        remove_programs();
        std::abort();
    }
    // Vertex shader end

    GLuint geomShaders[4];
    const std::string geomShadersStr[] = {
            getShaderPath("moonLanderPoint.glgs"),
            getShaderPath("moonLanderLineAdj.glgs"),
            getShaderPath("moonLanderTriangle.glgs"),
            getShaderPath("moonLanderLine.glgs")
    };

    for (size_t i = 0; i < std::size(geomShaders); ++i) {
        geomShaders[i] = loadShaderFromFile(geomShadersStr[i], GL_GEOMETRY_SHADER);

        if (geomShaders[i] == 0) {
            remove_programs();
            std::abort();
        }
    }

    // Fragment shader
    GLuint fragmentShader = loadShaderFromFile(
            getShaderPath("moonLander.glfs"), GL_FRAGMENT_SHADER);

    if (fragmentShader == 0) {
        remove_programs();
        std::abort();
    }
    // Fragment shader end

    auto shaderDeleter = [geomShaders, fragmentShader, vertexShader](){
        for (auto gs: geomShaders)
            glDeleteShader(gs);
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
    };

    for (int i = 0; i < std::size(programs); ++i) {
        glAttachShader(programs[i], fragmentShader);
        glAttachShader(programs[i], vertexShader);
        glAttachShader(programs[i], geomShaders[i]);
        if (GLenum error = glGetError(); error != GL_NO_ERROR) {
            std::abort();
        }

        glLinkProgram(programs[i]);
        GLint linkSuccess = GL_TRUE;
        glGetProgramiv(programs[i], GL_LINK_STATUS, &linkSuccess);
        if (linkSuccess != GL_TRUE) {
            printf("Error linking program %d!\n", programs[i]);
            utils::log::printProgramLog(programs[i]);
            shaderDeleter();
            remove_programs();
            std::abort();
        }
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    for (size_t i = 0; i < std::size(programs); ++i)
        glDeleteShader(geomShaders[i]);

    cur_program = programs[1];

    // Get block indices
    uniformPoints[0] = glGetUniformBlockIndex(programs[0], "TextureData");
    uniformPoints[1] = glGetUniformBlockIndex(programs[0], "Matrices");

    uniformLinesAdj[0] = glGetUniformBlockIndex(programs[1], "TextureData");
    uniformLinesAdj[1] = glGetUniformBlockIndex(programs[1], "Matrices");

    uniformTriangles[0] = glGetUniformBlockIndex(programs[2], "TextureData");
    uniformTriangles[1] = glGetUniformBlockIndex(programs[2], "Matrices");

    uniformLines[0] = glGetUniformBlockIndex(programs[3], "TextureData");
    uniformLines[1] = glGetUniformBlockIndex(programs[3], "Matrices");

    // Link each shader's uniform block to uniform binding point
    glUniformBlockBinding(programs[0], uniformPoints[0], 0); // Texture Data
    glUniformBlockBinding(programs[0], uniformPoints[1], 1); // Matrices

    glUniformBlockBinding(programs[1], uniformLinesAdj[0], 0);
    glUniformBlockBinding(programs[1], uniformLinesAdj[1], 1);

    glUniformBlockBinding(programs[2], uniformTriangles[0], 0);
    glUniformBlockBinding(programs[2], uniformTriangles[1], 1);

    glUniformBlockBinding(programs[3], uniformLines[0], 0);
    glUniformBlockBinding(programs[3], uniformLines[1], 1);

    // Create matrices buffer
    glGenBuffers(1, &matricesUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(mat4), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // Bind matrices to 1 index in binding point array
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, matricesUBO);

    //  Create texture data buffer
    size_t textureDataSize = sizeof(vec4);
    glGenBuffers(1, &textureDataUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, textureDataUBO);
    glBufferData(GL_UNIFORM_BUFFER, textureDataSize, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // Bind texture data to 0 index in binding point array
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, textureDataUBO);

    switchToLinesAdj();

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
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0,
                    sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        printf("Error while updating matricesUBO! %s\n", gluErrorString(error));
        utils::log::printProgramLog(cur_program);
        std::abort();
    }
}

void MoonLanderProgram::updateModel()
{
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4),
                    sizeof(glm::mat4), glm::value_ptr(modelMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        printf("Error while updating matricesUBO! %s\n", gluErrorString(error));
        utils::log::printProgramLog(cur_program);
        std::abort();
    }
}

void MoonLanderProgram::updateView()
{
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4),
                    sizeof(mat4), glm::value_ptr(viewMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        printf("Error while updating matricesUBO! %s\n", gluErrorString(error));
        utils::log::printProgramLog(cur_program);
        std::abort();
    }
}

void MoonLanderProgram::setColor(glm::vec4 color)
{
    this->color = color;
}

void MoonLanderProgram::updateColor()
{
    glUniform4f(colorLoc, color.x, color.y, color.z, color.w);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        printf("Error while updating textureDataUBO! %s\n",
               gluErrorString(error));
        utils::log::printProgramLog(cur_program);
        std::abort();
    }
}

void MoonLanderProgram::setTextureRendering(bool isTexture)
{
    isTextureRender = isTexture;
    glBindBuffer(GL_UNIFORM_BUFFER, textureDataUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, gl_bool_size, &isTextureRender);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        printf("Error while updating textureDataUBO! %s\n",
               gluErrorString(error));
        utils::log::printProgramLog(cur_program);
        std::abort();
    }
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

//TODO: fix resource leaking
MoonLanderProgram::~MoonLanderProgram() = default;

std::shared_ptr<MoonLanderProgram> MoonLanderProgram::instance = nullptr;

void MoonLanderProgram::switchToLinesAdj()
{
    cur_program = programs[1];
    glUseProgram(0);
    glUseProgram(cur_program);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        printf("Unable to switch to line adjacency primitive rendering! %s\n",
               gluErrorString(error));
        utils::log::printProgramLog(cur_program);
        std::abort();
    }
    rebindUniforms();
    setTexture(0);
}

void MoonLanderProgram::switchToLines()
{
    cur_program = programs[3];
    glUseProgram(0);
    glUseProgram(cur_program);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        printf("Unable to switch to line primitive rendering! %s\n",
               gluErrorString(error));
        utils::log::printProgramLog(cur_program);
        std::abort();
    }
    rebindUniforms();
    setTexture(0);
}

void MoonLanderProgram::switchToTriangles()
{
    cur_program = programs[2];
    glUseProgram(0);
    glUseProgram(cur_program);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        printf("Unable to switch to triangle primitive rendering! %s\n",
               gluErrorString(error));
        utils::log::printProgramLog(cur_program);
        std::abort();
    }
    rebindUniforms();
    setTexture(0);
}

void MoonLanderProgram::switchToPoints()
{
    cur_program = programs[0];
    glUseProgram(0);
    glUseProgram(cur_program);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        printf("Unable to switch to point primitive rendering! %s\n",
               gluErrorString(error));
        utils::log::printProgramLog(cur_program);
        std::abort();
    }
    rebindUniforms();
    setTexture(0);
}

void MoonLanderProgram::rebindUniforms()
{
    texLoc = glGetUniformLocation(cur_program, "ourTexture");
    if (texLoc == -1) {
        printf("%s is not a valid glsl program variable!\n", "ourTexture");
        std::abort();
    }

    colorLoc = glGetUniformLocation(cur_program, "inColor");
    if (colorLoc == -1) {
        printf("%s is not a valid glsl program variable!\n", "inColor");
        std::abort();
    }
}

void MoonLanderProgram::remove_programs()
{
    for (int i = 0; i < std::size(programs); ++i) {
        glDeleteProgram(programs[i]);
        programs[i] = 0;
    }
}

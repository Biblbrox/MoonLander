#include <GL/glew.h>
#include <utils.hpp>
#include "../include/moonlanderprogram.hpp"

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
}

bool MoonLanderProgram::loadProgram()
{
    using utils::getShaderPath;
    using utils::loadShaderFromFile;
    GLuint geomShaders[3];

    for (int i = 0; i < std::size(programs); ++i)
        programs[i] = glCreateProgram();

    // Vertex shader
    GLuint vertexShader = loadShaderFromFile(
            getShaderPath("moonLander.glvs"), GL_VERTEX_SHADER);

    if (vertexShader == 0) {
        remove_programs();
        std::abort();
    }
    // Vertex shader end

    // Geometry shader point
    geomShaders[0] = loadShaderFromFile(
            getShaderPath("moonLanderPoint.glgs"), GL_GEOMETRY_SHADER);

    if (geomShaders[0] == 0) {
        remove_programs();
        std::abort();
    }
    // Geometry shader point end

    // Geometry shader line
    geomShaders[1] = loadShaderFromFile(
            getShaderPath("moonLanderLine.glgs"), GL_GEOMETRY_SHADER);

    if (geomShaders[1] == 0) {
        remove_programs();
        std::abort();
    }
    // Geometry shader line end

    // Geometry shader triangle
    geomShaders[2] = loadShaderFromFile(
            getShaderPath("moonLanderTriangle.glgs"), GL_GEOMETRY_SHADER);

    if (geomShaders[2] == 0) {
        remove_programs();
        std::abort();
    }
    // Geometry shader point end

    // Fragment shader
    GLuint fragmentShader = loadShaderFromFile(
            getShaderPath("moonLander.glfs"), GL_FRAGMENT_SHADER);

    if (fragmentShader == 0) {
        remove_programs();
        std::abort();
    }
    // Fragment shader end

    for (int i = 0; i < std::size(programs); ++i) {
        glAttachShader(programs[i], fragmentShader);
        glAttachShader(programs[i], vertexShader);
        glAttachShader(programs[i], geomShaders[i]);

        glLinkProgram(programs[i]);
        GLint programSuccess = GL_TRUE;
        glGetProgramiv(programs[i], GL_LINK_STATUS, &programSuccess);
        if (programSuccess != GL_TRUE) {
            printf("Error linking program %d!\n", programs[i]);
            utils::log::printProgramLog(programs[i]);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            glDeleteShader(geomShaders[i]);
            glDeleteProgram(programs[i]);
            programs[i] = 0;
            std::abort();
        }
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    for (int i = 0; i < std::size(programs); ++i)
        glDeleteShader(geomShaders[i]);

    cur_program = programs[1];

    // Get block indices
    uniformPoints[0] = glGetUniformBlockIndex(programs[0], "TextureData");
    uniformPoints[1] = glGetUniformBlockIndex(programs[0], "Matrices");

    uniformLines[0] = glGetUniformBlockIndex(programs[1], "TextureData");
    uniformLines[1] = glGetUniformBlockIndex(programs[1], "Matrices");

    uniformTriangles[0] = glGetUniformBlockIndex(programs[2], "TextureData");
    uniformTriangles[1] = glGetUniformBlockIndex(programs[2], "Matrices");

    // Link each shader's uniform block to this uniform binding point
    glUniformBlockBinding(programs[0], uniformPoints[0], 0); // Texture Data
    glUniformBlockBinding(programs[0], uniformPoints[1], 1); // Matrices

    glUniformBlockBinding(programs[1], uniformLines[0], 0);
    glUniformBlockBinding(programs[1], uniformLines[1], 1);

    glUniformBlockBinding(programs[2], uniformTriangles[0], 0);
    glUniformBlockBinding(programs[2], uniformTriangles[1], 1);

    // Create matrices buffer
    glGenBuffers(1, &matricesUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // Bind matrices to 1 index in binding point array
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, matricesUBO);

    //  Create texture data buffer
    size_t textureDataSize = 4 + sizeof(glm::vec4);
    glGenBuffers(1, &textureDataUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, textureDataUBO);
    glBufferData(GL_UNIFORM_BUFFER, textureDataSize, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // Bind texture data to 0 index in binding point array
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, textureDataUBO);

    switchToTriangles();

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
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4),
                    sizeof(glm::mat4), glm::value_ptr(viewMatrix));
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
    glBindBuffer(GL_UNIFORM_BUFFER, textureDataUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0,
                    sizeof(glm::vec4), glm::value_ptr(color));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        printf("Error while updating textureDataUBO! %s\n", gluErrorString(error));
        utils::log::printProgramLog(cur_program);
        std::abort();
    }
}

void MoonLanderProgram::setTextureRendering(bool isTexture)
{
    isTextureRender = isTexture;
    glBindBuffer(GL_UNIFORM_BUFFER, textureDataUBO);
    glBufferSubData(GL_UNIFORM_BUFFER,
                    next_offset(sizeof(glm::vec4), gl_bool_size),
                    gl_bool_size, &isTextureRender);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        printf("Error while updating textureDataUBO! %s\n", gluErrorString(error));
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

MoonLanderProgram::~MoonLanderProgram() = default;

std::shared_ptr<MoonLanderProgram> MoonLanderProgram::instance = nullptr;

void MoonLanderProgram::switchToLines()
{
    cur_program = programs[1];
    glUseProgram(0);
    glUseProgram(cur_program);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
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
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
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
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
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
        printf( "%s is not a valid glsl program variable!\n", "ourTexture");
    }
}

void MoonLanderProgram::remove_programs()
{
    for (int i = 0; i < std::size(programs); ++i) {
        glDeleteProgram(programs[i]);
        programs[i] = 0;
    }
}

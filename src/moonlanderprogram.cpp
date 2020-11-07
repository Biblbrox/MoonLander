#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <utils/utils.hpp>
#include <boost/format.hpp>
#include "../include/exceptions/glexception.hpp"
#include "../include/utils/logger.hpp"
#include "../include/moonlanderprogram.hpp"

using utils::getShaderPath;
using utils::loadShaderFromFile;
using glm::mat4;
using glm::vec3;
using glm::vec4;
using utils::log::Logger;
using utils::log::Category;
using utils::log::program_log_file_name;
using utils::log::shader_log_file_name;
using boost::format;

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
    m_isTextureRender = false;
    m_texLoc = 0;
    m_curProgram = 0;
    m_colorLoc = 0;

    std::memset(m_geomShaders, 0, 4);
}

bool MoonLanderProgram::loadProgram()
{
    for (int i = 0; i < std::size(m_programs); ++i) {
        m_programs[i] = glCreateProgram();
        if (m_programs[i] == 0)
            throw GLException((format("Unable to create program %d\n")
                               % m_programs[i]).str(),
                              program_log_file_name(),
                              Category::INITIALIZATION_ERROR);
    }

    // Vertex shader
    m_vertexShader = loadShaderFromFile(
            getShaderPath("moonLander.glvs"), GL_VERTEX_SHADER);
    // Vertex shader end

    const std::string geomShadersStr[] = {
            getShaderPath("moonLanderPoint.glgs"),
            getShaderPath("moonLanderLineAdj.glgs"),
            getShaderPath("moonLanderTriangle.glgs"),
            getShaderPath("moonLanderLine.glgs")
    };

    for (size_t i = 0; i < std::size(m_geomShaders); ++i)
        m_geomShaders[i] = loadShaderFromFile(geomShadersStr[i], GL_GEOMETRY_SHADER);

    // Fragment shader
    m_fragmentShader = loadShaderFromFile(
            getShaderPath("moonLander.glfs"), GL_FRAGMENT_SHADER);
    // Fragment shader end

    for (int i = 0; i < std::size(m_programs); ++i) {
        glAttachShader(m_programs[i], m_fragmentShader);
        glAttachShader(m_programs[i], m_vertexShader);
        glAttachShader(m_programs[i], m_geomShaders[i]);
        if (GLenum error = glGetError(); error != GL_NO_ERROR) {
            utils::log::printProgramLog(m_programs[i]);
            throw GLException("Unable to attach shaders.\n",
                              program_log_file_name(),
                              Category::INITIALIZATION_ERROR);
        }

        glLinkProgram(m_programs[i]);
        GLint linkSuccess = GL_TRUE;
        glGetProgramiv(m_programs[i], GL_LINK_STATUS, &linkSuccess);
        if (linkSuccess != GL_TRUE) {
            utils::log::printProgramLog(m_programs[i]);
            throw GLException((format("Unable to link program: %d, %s\n")
                               % m_programs[i] % glGetError()).str(),
                              program_log_file_name(),
                              Category::INITIALIZATION_ERROR);
        }
    }

    m_curProgram = m_programs[1];

    // Get block indices
    m_uniformPoints[0] = glGetUniformBlockIndex(m_programs[0], "TextureData");
    m_uniformPoints[1] = glGetUniformBlockIndex(m_programs[0], "Matrices");

    m_uniformLinesAdj[0] = glGetUniformBlockIndex(m_programs[1], "TextureData");
    m_uniformLinesAdj[1] = glGetUniformBlockIndex(m_programs[1], "Matrices");

    m_uniformTriangles[0] = glGetUniformBlockIndex(m_programs[2], "TextureData");
    m_uniformTriangles[1] = glGetUniformBlockIndex(m_programs[2], "Matrices");

    m_uniformLines[0] = glGetUniformBlockIndex(m_programs[3], "TextureData");
    m_uniformLines[1] = glGetUniformBlockIndex(m_programs[3], "Matrices");

    // Link each shader's uniform block to uniform binding point
    glUniformBlockBinding(m_programs[0], m_uniformPoints[0], 0); // Texture Data
    glUniformBlockBinding(m_programs[0], m_uniformPoints[1], 1); // Matrices

    glUniformBlockBinding(m_programs[1], m_uniformLinesAdj[0], 0);
    glUniformBlockBinding(m_programs[1], m_uniformLinesAdj[1], 1);

    glUniformBlockBinding(m_programs[2], m_uniformTriangles[0], 0);
    glUniformBlockBinding(m_programs[2], m_uniformTriangles[1], 1);

    glUniformBlockBinding(m_programs[3], m_uniformLines[0], 0);
    glUniformBlockBinding(m_programs[3], m_uniformLines[1], 1);

    // Create matrices buffer
    glGenBuffers(1, &m_matricesUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_matricesUBO);
    glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(mat4), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // Bind matrices to 1 index in binding point array
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_matricesUBO);

    //  Create texture data buffer
    size_t textureDataSize = sizeof(vec4);
    glGenBuffers(1, &m_textureDataUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_textureDataUBO);
    glBufferData(GL_UNIFORM_BUFFER, textureDataSize, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // Bind texture data to 0 index in binding point array
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_textureDataUBO);

    switchToLinesAdj();

    return true;
}

void MoonLanderProgram::setProjection(glm::mat4 matrix)
{
    m_projectionMatrix = matrix;
}

void MoonLanderProgram::setView(glm::mat4 matrix)
{
    m_viewMatrix = matrix;
}

void MoonLanderProgram::setModel(glm::mat4 matrix)
{
    m_modelMatrix = matrix;
}

void MoonLanderProgram::leftMultModel(glm::mat4 matrix)
{
    m_modelMatrix = matrix * m_modelMatrix;
}

void MoonLanderProgram::leftMultView(glm::mat4 matrix)
{
    m_viewMatrix = matrix * m_viewMatrix;
}

void MoonLanderProgram::leftMultProjection(glm::mat4 matrix)
{
    m_projectionMatrix = matrix * m_projectionMatrix;
}

void MoonLanderProgram::updateProjection()
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_matricesUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0,
                    sizeof(glm::mat4), glm::value_ptr(m_projectionMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_curProgram);
        throw GLException((format("Error while updating matricesUBO(m_projectionMatrix)! %s\n")
                           % gluErrorString(error)).str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }
}

void MoonLanderProgram::updateModel()
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_matricesUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4),
                    sizeof(glm::mat4), glm::value_ptr(m_modelMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_curProgram);
        throw GLException((format("Error while updating matricesUBO(m_modelMatrix)! %s\n")
                           % gluErrorString(error)).str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }
}

void MoonLanderProgram::updateView()
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_matricesUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4),
                    sizeof(mat4), glm::value_ptr(m_viewMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_curProgram);
        throw GLException((format("Error while updating"
                                  " matricesUBO(m_viewMatrix)! %s\n")
                           % gluErrorString(error)).str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }
}

void MoonLanderProgram::setColor(glm::vec4 color)
{
    m_color = color;
}

void MoonLanderProgram::updateColor()
{
    glUniform4f(m_colorLoc, m_color.x, m_color.y, m_color.z, m_color.w);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_curProgram);
        throw GLException((format("Error while updating color! %s\n")
                           % gluErrorString(error)).str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }
}

void MoonLanderProgram::setTextureRendering(bool isTexture)
{
    m_isTextureRender = isTexture;
    glBindBuffer(GL_UNIFORM_BUFFER, m_textureDataUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, gl_bool_size, &m_isTextureRender);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_curProgram);
        throw GLException((format("Error while updating "
                                  "textureDataUBO(m_isTextureRender)! %s\n")
                           % gluErrorString(error)).str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }
}

void MoonLanderProgram::setTexture(int texture)
{
    glUniform1i(m_texLoc, texture);
}

glm::mat4 MoonLanderProgram::getView() const
{
    return m_viewMatrix;
}

glm::mat4 MoonLanderProgram::getProjection() const
{
    return m_projectionMatrix;
}

glm::mat4 MoonLanderProgram::getModel() const
{
    return m_modelMatrix;
}

MoonLanderProgram::~MoonLanderProgram()
{
    remove_shaders();
    remove_programs();
    free_buffers();
}

std::shared_ptr<MoonLanderProgram> MoonLanderProgram::instance = nullptr;

void MoonLanderProgram::switchToLinesAdj()
{
    m_curProgram = m_programs[1];
    glUseProgram(0);
    glUseProgram(m_curProgram);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_curProgram);
        throw GLException((format("Unable to switch to line adjacency"
                                  " primitive rendering! %s\n")
                           % gluErrorString(error)).str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }
    rebindUniforms();
    setTexture(0);
}

void MoonLanderProgram::switchToLines()
{
    m_curProgram = m_programs[3];
    glUseProgram(0);
    glUseProgram(m_curProgram);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_curProgram);
        throw GLException((format("Unable to switch to line"
                                  " primitive rendering! %s\n")
                           % gluErrorString(error)).str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }
    rebindUniforms();
    setTexture(0);
}

void MoonLanderProgram::switchToTriangles()
{
    m_curProgram = m_programs[2];
    glUseProgram(0);
    glUseProgram(m_curProgram);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_curProgram);
        throw GLException((format("Unable to switch to triangle"
                                  " primitive rendering! %s\n")
                           % gluErrorString(error)).str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }
    rebindUniforms();
    setTexture(0);
}

void MoonLanderProgram::switchToPoints()
{
    m_curProgram = m_programs[0];
    glUseProgram(0);
    glUseProgram(m_curProgram);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_curProgram);
        throw GLException((format("Unable to switch to points"
                                  " primitive rendering! %s\n")
                           % gluErrorString(error)).str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }
    rebindUniforms();
    setTexture(0);
}

void MoonLanderProgram::rebindUniforms()
{
    m_texLoc = glGetUniformLocation(m_curProgram, "ourTexture");
    if (m_texLoc == -1) {
        utils::log::printProgramLog(m_curProgram);
        throw GLException((format("%s is not a valid glsl program variable!\n")
                           % "ourTexture").str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }

    m_colorLoc = glGetUniformLocation(m_curProgram, "inColor");
    if (m_colorLoc == -1) {
        utils::log::printProgramLog(m_curProgram);
        throw GLException((format("%s is not a valid glsl program variable!\n")
                           % "inColor").str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }
}

void MoonLanderProgram::remove_programs()
{
    for (int i = 0; i < std::size(m_programs); ++i) {
        if (glIsProgram(m_programs[i])) {
            glDeleteProgram(m_programs[i]);
            m_programs[i] = 0;
        }
    }
}

void MoonLanderProgram::free_buffers()
{
    glDeleteBuffers(1, &m_matricesUBO);
    glDeleteBuffers(1, &m_textureDataUBO);

    m_matricesUBO = m_textureDataUBO = 0;
}

void MoonLanderProgram::remove_shaders()
{
    for (auto gs: m_geomShaders)
        if (glIsShader(gs))
            glDeleteShader(gs);

    if (glIsShader(m_fragmentShader))
        glDeleteShader(m_fragmentShader);

    if (glIsShader(m_vertexShader))
        glDeleteShader(m_vertexShader);
}

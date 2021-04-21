#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "utils/utils.hpp"
#include "boost/format.hpp"
#include "exceptions/glexception.hpp"
#include "utils/logger.hpp"
#include "moonlanderprogram.hpp"

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
    m_programID = 0;
    m_colorLoc = 0;

    std::memset(m_geomShaders, 0, 4);
}

void MoonLanderProgram::loadProgram()
{
    for (int i = 0; i < std::size(m_fbprograms); ++i) {
        m_fbprograms[i] = glCreateProgram();
        if (m_fbprograms[i] == 0)
            throw GLException((format("Unable to create program %d\n")
                               % m_fbprograms[i]).str(),
                              program_log_file_name(),
                              Category::INITIALIZATION_ERROR);
    }

    // Vertex shader
    m_vertexShader = loadShaderFromFile(
            getShaderPath("framebuffer/moonLander.glvs"), GL_VERTEX_SHADER);
    // Vertex shader end

    const std::string geomShadersStr[] = {
            getShaderPath("framebuffer/moonLanderPoint.glgs"),
            getShaderPath("framebuffer/moonLanderLineAdj.glgs"),
            getShaderPath("framebuffer/moonLanderTriangle.glgs"),
            getShaderPath("framebuffer/moonLanderLine.glgs")
    };

    for (size_t i = 0; i < std::size(m_geomShaders); ++i)
        m_geomShaders[i] = loadShaderFromFile(geomShadersStr[i], GL_GEOMETRY_SHADER);

    // Fragment shader
    m_fragmentShader = loadShaderFromFile(
            getShaderPath("framebuffer/moonLander.glfs"), GL_FRAGMENT_SHADER);
    // Fragment shader end

    for (int i = 0; i < std::size(m_fbprograms); ++i) {
        glAttachShader(m_fbprograms[i], m_fragmentShader);
        glAttachShader(m_fbprograms[i], m_vertexShader);
        glAttachShader(m_fbprograms[i], m_geomShaders[i]);
        if (GLenum error = glGetError(); error != GL_NO_ERROR) {
            utils::log::printProgramLog(m_fbprograms[i]);
            throw GLException("Unable to attach shaders.\n",
                              program_log_file_name(),
                              Category::INITIALIZATION_ERROR);
        }

        glLinkProgram(m_fbprograms[i]);
        GLint linkSuccess = GL_TRUE;
        glGetProgramiv(m_fbprograms[i], GL_LINK_STATUS, &linkSuccess);
        if (linkSuccess != GL_TRUE) {
            utils::log::printProgramLog(m_fbprograms[i]);
            throw GLException((format("Unable to link program: %d, %s\n")
                               % m_fbprograms[i] % glGetError()).str(),
                              program_log_file_name(),
                              Category::INITIALIZATION_ERROR);
        }
    }

    for (auto gs: m_geomShaders)
        if (glIsShader(gs))
            glDeleteShader(gs);

    if (glIsShader(m_fragmentShader))
        glDeleteShader(m_fragmentShader);

    if (glIsShader(m_vertexShader))
        glDeleteShader(m_vertexShader);

    m_programID = m_fbprograms[1];



    // Create screen program
    m_screenProgram = glCreateProgram();
    if (m_screenProgram == 0)
        throw GLException((format("Unable to create program %d\n")
                           % m_screenProgram).str(),
                          program_log_file_name(),
                          Category::INITIALIZATION_ERROR);

    // Vertex shader
    m_vertexShader = loadShaderFromFile(
            getShaderPath("screen/moonLander.glvs"), GL_VERTEX_SHADER);
    // Vertex shader end

    // Fragment shader
    m_fragmentShader = loadShaderFromFile(
            getShaderPath("screen/moonLander.glfs"), GL_FRAGMENT_SHADER);
    // Fragment shader end

    glAttachShader(m_screenProgram, m_fragmentShader);
    glAttachShader(m_screenProgram, m_vertexShader);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_screenProgram);
        throw GLException("Unable to attach shaders.\n",
                          program_log_file_name(),
                          Category::INITIALIZATION_ERROR);
    }
    
    glLinkProgram(m_screenProgram);
    GLint linkSuccess = GL_TRUE;
    glGetProgramiv(m_screenProgram, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess != GL_TRUE) {
        utils::log::printProgramLog(m_screenProgram);
        throw GLException((format("Unable to link program: %d, %s\n")
                           % m_screenProgram % glGetError()).str(),
                          program_log_file_name(),
                          Category::INITIALIZATION_ERROR);
    }

    if (glIsShader(m_fragmentShader))
        glDeleteShader(m_fragmentShader);

    if (glIsShader(m_vertexShader))
        glDeleteShader(m_vertexShader);




    // Create final program
    m_finalProgram = glCreateProgram();
    if (m_finalProgram == 0)
        throw GLException((format("Unable to create program %d\n")
                           % m_finalProgram).str(),
                          program_log_file_name(),
                          Category::INITIALIZATION_ERROR);

    // Vertex shader
    m_vertexShader = loadShaderFromFile(
            getShaderPath("screen/moonLander.glvs"), GL_VERTEX_SHADER);
    // Vertex shader end

    // Fragment shader
    m_fragmentShader = loadShaderFromFile(
            getShaderPath("screen/moonLanderFinal.glfs"), GL_FRAGMENT_SHADER);
    // Fragment shader end

    glAttachShader(m_finalProgram, m_fragmentShader);
    glAttachShader(m_finalProgram, m_vertexShader);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_finalProgram);
        throw GLException("Unable to attach shaders.\n",
                          program_log_file_name(),
                          Category::INITIALIZATION_ERROR);
    }

    glLinkProgram(m_finalProgram);
    linkSuccess = GL_TRUE;
    glGetProgramiv(m_finalProgram, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess != GL_TRUE) {
        utils::log::printProgramLog(m_finalProgram);
        throw GLException((format("Unable to link program: %d, %s\n")
                           % m_finalProgram % glGetError()).str(),
                          program_log_file_name(),
                          Category::INITIALIZATION_ERROR);
    }

    if (glIsShader(m_fragmentShader))
        glDeleteShader(m_fragmentShader);

    if (glIsShader(m_vertexShader))
        glDeleteShader(m_vertexShader);



    // Get block indices
    m_uniformPoints[0] = glGetUniformBlockIndex(m_fbprograms[0], "TextureData");
    m_uniformPoints[1] = glGetUniformBlockIndex(m_fbprograms[0], "Matrices");

    m_uniformLinesAdj[0] = glGetUniformBlockIndex(m_fbprograms[1], "TextureData");
    m_uniformLinesAdj[1] = glGetUniformBlockIndex(m_fbprograms[1], "Matrices");

    m_uniformTriangles[0] = glGetUniformBlockIndex(m_fbprograms[2], "TextureData");
    m_uniformTriangles[1] = glGetUniformBlockIndex(m_fbprograms[2], "Matrices");

    m_uniformLines[0] = glGetUniformBlockIndex(m_fbprograms[3], "TextureData");
    m_uniformLines[1] = glGetUniformBlockIndex(m_fbprograms[3], "Matrices");

    // Link each shader's uniform block to uniform binding point
    glUniformBlockBinding(m_fbprograms[0], m_uniformPoints[0], 0); // Texture Data
    glUniformBlockBinding(m_fbprograms[0], m_uniformPoints[1], 1); // Matrices

    glUniformBlockBinding(m_fbprograms[1], m_uniformLinesAdj[0], 0);
    glUniformBlockBinding(m_fbprograms[1], m_uniformLinesAdj[1], 1);

    glUniformBlockBinding(m_fbprograms[2], m_uniformTriangles[0], 0);
    glUniformBlockBinding(m_fbprograms[2], m_uniformTriangles[1], 1);

    glUniformBlockBinding(m_fbprograms[3], m_uniformLines[0], 0);
    glUniformBlockBinding(m_fbprograms[3], m_uniformLines[1], 1);

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
}

void MoonLanderProgram::setColor(glm::vec4 color)
{
    m_color = color;
}

void MoonLanderProgram::updateColor()
{
    glUniform4f(m_colorLoc, m_color.x, m_color.y, m_color.z, m_color.w);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_programID);
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
        utils::log::printProgramLog(m_programID);
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

MoonLanderProgram::~MoonLanderProgram()
{
    remove_programs();
    free_buffers();
}

std::shared_ptr<MoonLanderProgram> MoonLanderProgram::instance = nullptr;

void MoonLanderProgram::useScreenProgram()
{
    m_programID = m_screenProgram;
    glUseProgram(0);
    glUseProgram(m_programID);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_programID);
        throw GLException((format("Unable to switch to screen program"
                                  " primitive rendering! %s\n")
                           % gluErrorString(error)).str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }

    m_texLoc = glGetUniformLocation(m_programID, "image");
    if (m_texLoc == -1) {
        utils::log::printProgramLog(m_programID);
        throw GLException((format("%s is not a valid glsl program variable!\n")
                           % "image").str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }

    setTexture(0);
}

void MoonLanderProgram::useFinalProgram()
{
    m_programID = m_finalProgram;
    glUseProgram(0);
    glUseProgram(m_programID);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_programID);
        throw GLException((format("Unable to switch to screen program"
                                  " primitive rendering! %s\n")
                           % gluErrorString(error)).str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }

    m_texLoc = glGetUniformLocation(m_programID, "image");
    if (m_texLoc == -1) {
        utils::log::printProgramLog(m_programID);
        throw GLException((format("%s is not a valid glsl program variable!\n")
                           % "image").str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }

    setTexture(0);
}

void MoonLanderProgram::switchToLinesAdj()
{
    m_programID = m_fbprograms[1];
    glUseProgram(0);
    glUseProgram(m_programID);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_programID);
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
    m_programID = m_fbprograms[3];
    glUseProgram(0);
    glUseProgram(m_programID);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_programID);
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
    m_programID = m_fbprograms[2];
    glUseProgram(0);
    glUseProgram(m_programID);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_programID);
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
    m_programID = m_fbprograms[0];
    glUseProgram(0);
    glUseProgram(m_programID);
    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_programID);
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
    m_texLoc = glGetUniformLocation(m_programID, "image");
    if (m_texLoc == -1) {
        utils::log::printProgramLog(m_programID);
        throw GLException((format("%s is not a valid glsl program variable!\n")
                           % "image").str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }

    m_colorLoc = glGetUniformLocation(m_programID, "inColor");
    if (m_colorLoc == -1) {
        utils::log::printProgramLog(m_programID);
        throw GLException((format("%s is not a valid glsl program variable!\n")
                           % "inColor").str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }
}

void MoonLanderProgram::remove_programs()
{
    for (int i = 0; i < std::size(m_fbprograms); ++i) {
        if (glIsProgram(m_fbprograms[i])) {
            glDeleteProgram(m_fbprograms[i]);
            m_fbprograms[i] = 0;
        }
    }
}

void MoonLanderProgram::free_buffers()
{
    glDeleteBuffers(1, &m_matricesUBO);
    glDeleteBuffers(1, &m_textureDataUBO);

    m_matricesUBO = m_textureDataUBO = 0;
}


void MoonLanderProgram::updateProjection()
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_matricesUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0,
                    sizeof(glm::mat4), glm::value_ptr(m_projectionMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        utils::log::printProgramLog(m_programID);
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
        utils::log::printProgramLog(m_programID);
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
        utils::log::printProgramLog(m_programID);
        throw GLException((format("Error while updating"
                                  " matricesUBO(m_viewMatrix)! %s\n")
                           % gluErrorString(error)).str(),
                          program_log_file_name(),
                          Category::INTERNAL_ERROR);
    }
}

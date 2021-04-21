#include <boost/format.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "systems/renderersystem.hpp"
#include "components/levelcomponent.hpp"
#include "components/spritecomponent.hpp"
#include "render/render.hpp"
#include "utils/logger.hpp"
#include "exceptions/glexception.hpp"

using utils::log::Logger;
using boost::format;
using utils::log::program_log_file_name;
using utils::log::Category;
using glm::mat4;
using glm::vec3;
using glm::scale;

static GLuint genTexture(GLuint width, GLuint height,
                         bool msaa = false, size_t samples = 4)
{
    GLuint texture;
    glGenTextures(1, &texture);

    if (msaa) {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples,
                                GL_RGB, width, height, GL_TRUE);
    } else {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0,
                     GL_RGBA, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (msaa)
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    else
        glBindTexture(GL_TEXTURE_2D, 0);

    if (GLuint error = glGetError(); error != GL_NO_ERROR)
        throw GLException((format("Unable to generate texture! %1%\n")
                           % gluErrorString(error)).str(),
                          program_log_file_name(), Category::INTERNAL_ERROR);

    return texture;
}

static GLuint genRbo(GLuint width, GLuint height, bool msaa = false,
                     size_t samples = 4)
{
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    return rbo;
}


void RendererSystem::drawLevel()
{
    auto levelEntities = getEntitiesByTag<LevelComponent>();
    auto program = MoonLanderProgram::getInstance();
    program->setTextureRendering(false);
    for (const auto& [key, en]: levelEntities) {
        GLfloat scale_factor = en->getComponent<LevelComponent>()->scale_factor;
        GLfloat invScale = 1.f / scale_factor;

        glm::mat4 scaling = glm::scale(glm::mat4(1.f),
                                       glm::vec3(scale_factor, scale_factor,1.f));
        program->leftMultModel(scaling);
        program->updateModel();

        program->switchToPoints();
        program->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
        program->updateColor();
        render::drawDots(en->getComponent<LevelComponent>()->stars);

        glLineWidth(1.f);
        program->switchToLinesAdj();
        program->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
        program->updateColor();
        render::drawLinen(en->getComponent<LevelComponent>()->points, true);

        glLineWidth(4.f);
        program->switchToLines();
        program->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
        program->updateColor();
        render::drawLinen(en->getComponent<LevelComponent>()->platforms);

        scaling[0][0] = invScale;
        scaling[1][1] = invScale;
        scaling[2][2] = invScale;
        program->leftMultModel(scaling);
        program->updateModel();
    }

    if (GLenum error = glGetError(); error != GL_NO_ERROR)
        throw GLException((format("\n\tRender while drawing level: %1%\n")
                           % glewGetErrorString(error)).str(),
                          program_log_file_name(), Category::INTERNAL_ERROR);
}

void RendererSystem::drawSprites()
{
    auto sprites = getEntitiesByTag<SpriteComponent>();
    auto program = MoonLanderProgram::getInstance();
    program->switchToTriangles();
    program->setTextureRendering(true);
    for (const auto& [key, en]: sprites) {
        render::drawTexture(*program, *en->getComponent<SpriteComponent>()->sprite,
                           en->getComponent<PositionComponent>()->x,
                           en->getComponent<PositionComponent>()->y,
                           en->getComponent<PositionComponent>()->angle,
                           en->getComponent<PositionComponent>()->scale_factor);
    }
    if (GLenum error = glGetError(); error != GL_NO_ERROR)
        throw GLException((format("\n\tRender while drawing level: %1%\n")
                           % glewGetErrorString(error)).str(),
                          program_log_file_name(), Category::INTERNAL_ERROR);
}

void RendererSystem::drawText()
{
    auto textComponents = getEntitiesByTag<TextComponent>();
    auto program = MoonLanderProgram::getInstance();
    program->switchToTriangles();
    program->setTextureRendering(true);
    for (const auto& [key, en]: textComponents) {
        render::drawTexture(*program, *en->getComponent<TextComponent>()->texture,
                           en->getComponent<PositionComponent>()->x,
                           en->getComponent<PositionComponent>()->y,
                           en->getComponent<PositionComponent>()->angle,
                           en->getComponent<PositionComponent>()->scale_factor);
    }
    if (GLenum error = glGetError(); error != GL_NO_ERROR)
        throw GLException((format("\n\tRender while drawing level: %1%\n")
                           % glewGetErrorString(error)).str(),
                          program_log_file_name(), Category::INTERNAL_ERROR);
}

void RendererSystem::renderSceneQuad()
{
    static GLuint quadVAO = 0;
    static GLuint quadVBO = 0;

    if (quadVAO == 0) {
        float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void RendererSystem::update_state(size_t delta)
{
    auto program = MoonLanderProgram::getInstance();

    // Use framebuffer program
    glBindFramebuffer(GL_FRAMEBUFFER, m_directBuffer);
    glActiveTexture(GL_TEXTURE0);
    program->setTexture(0);
    drawLevel();
    drawSprites();
    drawText();

    program->useScreenProgram();
    size_t amounts = 2;
    bool horizontal = true;
    bool first_iter = true;
    program->setBool("blur", true);
    for (size_t i = 0; i < amounts; ++i) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_pingpongFBO[horizontal]);
        program->setBool("horizontal", horizontal);
        glBindTexture(GL_TEXTURE_2D, first_iter ? m_directTexture
                                                : m_pingpongTextures[!horizontal]);
        // Draw scene's texture
        renderSceneQuad();

        horizontal = !horizontal;
        if (first_iter)
            first_iter = false;
    }

    // Combine m_blurBuffer with m_directBuffer
//    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    program->useFinalProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_directTexture);
    program->setInt("image", 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_pingpongTextures[!horizontal]);
    program->setInt("blured_image", 1);

    renderSceneQuad();


    for (unsigned int fb : m_pingpongFBO) {
        glBindFramebuffer(GL_FRAMEBUFFER, fb);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, m_directBuffer);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

RendererSystem::RendererSystem()
{
    GLuint rbo;
    auto program = MoonLanderProgram::getInstance();
    // Create buffer without blur
    glGenFramebuffers(1, &m_directBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_directBuffer);
    // Create color texture attachment
    GLfloat screen_width = utils::getScreenWidth<GLfloat>();
    GLfloat screen_height = utils::getScreenHeight<GLfloat>();
    m_directTexture = genTexture(screen_width, screen_height);
    rbo = genRbo(screen_width, screen_height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_directTexture, 0);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
//                              GL_RENDERBUFFER, rbo);

    // Create buffer for blurred texture
    glGenFramebuffers(1, &m_blurBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_blurBuffer);
    // Create color texture attachment
    m_blurTexture = genTexture(screen_width, screen_height);
    rbo = genRbo(screen_width, screen_height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_blurTexture, 0);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
//                              GL_RENDERBUFFER, rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(2, m_pingpongFBO);
    glGenTextures(2, m_pingpongTextures);
    for (size_t i = 0; i < 2; ++i) {
        m_pingpongTextures[i] = genTexture(screen_width, screen_height);
        glBindFramebuffer(GL_FRAMEBUFFER, m_pingpongFBO[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, m_pingpongTextures[i], 0);
    }
}

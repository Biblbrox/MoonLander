#include <boost/format.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "systems/renderersystem.hpp"
#include "components/levelcomponent.hpp"
#include "components/spritecomponent.hpp"
#include "render/render.hpp"
#include "../include/utils/logger.hpp"
#include "../include/exceptions/glexception.hpp"

using utils::log::Logger;
using boost::format;
using utils::log::program_log_file_name;
using utils::log::Category;
using glm::mat4;
using glm::vec3;
using glm::scale;

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

void RendererSystem::update_state(size_t delta)
{
    drawLevel();
    drawSprites();
    drawText();
}

RendererSystem::RendererSystem()
{

}

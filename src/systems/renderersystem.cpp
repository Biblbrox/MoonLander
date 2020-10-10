#include "systems/renderersystem.hpp"
#include "components/levelcomponent.hpp"
#include "components/spritecomponent.hpp"
#include "render.hpp"

using utils::log::Logger;
using boost::format;
using utils::program_log_file_name;
using utils::log::Category;

void RendererSystem::drawLevel()
{
    auto levelEntities = getEntitiesByTag<LevelComponent>();
    auto program = MoonLanderProgram::getInstance();
    program->setTextureRendering(false);
    for (const auto& [key, en]: levelEntities) {
        GLfloat scale_factor = en->getComponent<LevelComponent>()->scale_factor;

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

        if (GLenum error = glGetError(); error != GL_NO_ERROR) {
            Logger::write(program_log_file_name(), Category::INTERNAL_ERROR,
                          (format("\n\tRender error: %1%\n")
                           % glewGetErrorString(error)).str());
            std::abort();
        }

        scaling = glm::scale(
                glm::mat4(1.f),glm::vec3(1 / scale_factor,
                                         1 / scale_factor,1.f));
        program->leftMultModel(scaling);
        program->updateModel();
    }
}

void RendererSystem::drawSprites()
{
    auto sprites = getEntitiesByTag<SpriteComponent>();
    auto program = MoonLanderProgram::getInstance();
    for (const auto& [key, en]: sprites) {
        render::drawSprite(*en->getComponent<SpriteComponent>()->sprite,
                           en->getComponent<PositionComponent>()->x,
                           en->getComponent<PositionComponent>()->y,
                           en->getComponent<PositionComponent>()->angle,
                           en->getComponent<PositionComponent>()->scale_factor);
    }
}

void RendererSystem::drawText()
{
    auto textComponents = getEntitiesByTag<TextComponent>();
    auto program = MoonLanderProgram::getInstance();
    for (const auto& [key, en]: textComponents) {
        render::drawSprite(*en->getComponent<TextComponent>()->texture,
                           en->getComponent<PositionComponent>()->x,
                           en->getComponent<PositionComponent>()->y,
                           en->getComponent<PositionComponent>()->angle,
                           en->getComponent<PositionComponent>()->scale_factor);
    }
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

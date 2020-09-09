#include "systems/renderersystem.hpp"
#include <algorithm>
#include "components/levelcomponent.hpp"
#include "components/spritecomponent.hpp"
#include "render.hpp"

void RendererSystem::drawLevel()
{
    auto levelEntities = getEntitiesByTag<LevelComponent>();
    auto program = MoonLanderProgram::getInstance();
    for (auto& [key, en]: levelEntities) {
        GLfloat scale_factor = en->getComponent<LevelComponent>()->scale_factor;
        program->setTextureRendering(false);

        glm::mat4 scaling = glm::scale(glm::mat4(1.f),
                                       glm::vec3(scale_factor, scale_factor,1.f));
        program->leftMultModel(scaling);
        program->updateModel();

        program->switchToPoints();
        program->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
        program->updateColor();
        render::drawDots(en->getComponent<LevelComponent>()->stars);

        program->switchToLines();
        program->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
        program->updateColor();
        render::drawLinen(en->getComponent<LevelComponent>()->points);

        scaling = glm::scale(glm::mat4(1.f),
                             glm::vec3(1 / scale_factor, 1 / scale_factor,1.f));
        program->leftMultModel(scaling);
        program->updateModel();
    }
}

void RendererSystem::drawSprites()
{
    auto sprites = getEntitiesByTag<SpriteComponent>();
    auto program = MoonLanderProgram::getInstance();
    for (auto& [key, en]: sprites) {
        render::drawSprite(program, *en->getComponent<SpriteComponent>()->sprite,
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
    for (auto& [key, en]: textComponents) {
        render::drawSprite(program, *en->getComponent<TextComponent>()->texture,
                         en->getComponent<PositionComponent>()->x,
                         en->getComponent<PositionComponent>()->y,
                         en->getComponent<PositionComponent>()->angle,
                         en->getComponent<PositionComponent>()->scale_factor);
    }
}

void RendererSystem::update(size_t delta)
{
    drawLevel();
    drawSprites();
    drawText();
}

RendererSystem::RendererSystem()
{

}

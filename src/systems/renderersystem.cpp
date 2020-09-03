#include "systems/renderersystem.hpp"
#include <algorithm>
#include "components/levelcomponent.hpp"
#include "components/spritecomponent.hpp"
#include "renderer.hpp"

void RendererSystem::drawLevel()
{
    auto levelEntities = getEntitiesByTag<LevelComponent>();
    auto program = MoonLanderProgram::getInstance();
    program->setTextureRendering(false);
    for (auto& [key, en]: levelEntities) {
        Renderer::render(program, en->getComponent<LevelComponent>()->points,
                         en->getComponent<LevelComponent>()->stars,
                         en->getComponent<LevelComponent>()->scale_factor);
    }
}

void RendererSystem::drawSprites()
{
    auto sprites = getEntitiesByTag<SpriteComponent>();
    auto program = MoonLanderProgram::getInstance();
    program->setTextureRendering(true);
    for (auto& [key, en]: sprites) {
        Renderer::render(program, *en->getComponent<SpriteComponent>()->sprite,
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
    program->setTextureRendering(true);
    for (auto& [key, en]: textComponents) {
        Renderer::render(program, *en->getComponent<TextComponent>()->texture,
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

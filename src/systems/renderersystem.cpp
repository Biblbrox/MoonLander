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
        render::drawLevel(program, en->getComponent<LevelComponent>()->points,
                         en->getComponent<LevelComponent>()->stars,
                         en->getComponent<LevelComponent>()->scale_factor);
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

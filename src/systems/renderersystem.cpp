#include "systems/renderersystem.hpp"
#include <algorithm>
#include "components/levelcomponent.hpp"
#include "components/spritecomponent.hpp"
#include "renderer.hpp"

void RendererSystem::drawLevel()
{
    auto levelEntities = getEntitiesByTag<LevelComponent>();
    program.setTextureRendering(false);
    for (auto& [key, en]: levelEntities) {
        Renderer::render(program, en->getComponent<LevelComponent>()->points,
                         en->getComponent<LevelComponent>()->stars,
                         en->getComponent<LevelComponent>()->scale_factor);
    }
}

void RendererSystem::drawSprites()
{
    auto sprites = getEntitiesByTag<SpriteComponent>();
    program.setTextureRendering(true);
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
    program.setTextureRendering(true);
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
    glViewport(0.f, 0.f, screen_width, screen_height);
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    drawLevel();
    drawSprites();
    drawText();
}

RendererSystem::RendererSystem()
{
    screen_width = utils::getScreenWidth<GLuint>();
    screen_height = utils::getScreenHeight<GLuint>();

    program.loadProgram();
    program.bind();
    program.setProjection(glm::ortho<GLfloat>(
            0.0f, screen_width, screen_height,
            0.0f, 1.0f, -1.0f));
    program.setModel(glm::mat4(1.f));
    program.setView(glm::mat4(1.f));
    program.setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
    program.updateModel();
    program.updateView();
    program.updateProjection();
    program.updateColor();
    program.setTexture(0);
}

#include "systems/renderersystem.hpp"
#include <algorithm>
#include "components/levelcomponent.h"
#include "components/spritecomponent.hpp"
#include "renderer.h"

void RendererSystem::update(size_t delta)
{
    //glm::mat4 scaling = glm::scale(glm::mat4(1.f), glm::vec3(scale_factor, scale_factor, 1.f));
   // program.leftMultView(scaling);
    //program.updateView();

    auto levelEntities = getEntitiesByTag<LevelComponent>();
    program.setTextureRendering(false);
    for (auto& [key, en]: levelEntities) {
        Renderer::render(program, en->getComponent<LevelComponent>()->points,
                         en->getComponent<LevelComponent>()->stars,
                         scale_factor);
    }

    auto sprites = getEntitiesByTag<SpriteComponent>();
    program.setTextureRendering(true);
    for (auto& [key, en]: sprites) {
        Renderer::render(program, *en->getComponent<SpriteComponent>()->sprite,
                               en->getComponent<PositionComponent>()->x,
                               en->getComponent<PositionComponent>()->y,
                               en->getComponent<PositionComponent>()->angle,
                               en->getComponent<PositionComponent>()->scale_factor);
    }

    auto textComponents = getEntitiesByTag<TextComponent>();
    for (auto& [key, en]: textComponents) {
        Renderer::render(program, *en->getComponent<TextComponent>()->texture,
                                en->getComponent<PositionComponent>()->x,
                                en->getComponent<PositionComponent>()->y,
                                en->getComponent<PositionComponent>()->angle,
                                en->getComponent<PositionComponent>()->scale_factor);
    }

    //scaling = glm::scale(glm::mat4(1.f), glm::vec3(1 / scale_factor, 1 / scale_factor, 1.f));
    //program.leftMultView(scaling);
   // program.updateView();
}

RendererSystem::RendererSystem()
{
    auto screen_width = Utils::getScreenWidth<GLuint>();
    auto screen_height = Utils::getScreenHeight<GLuint>();

    program.loadProgram();
    program.bind();
    program.setProjection(glm::ortho<GLfloat>(0.0f, screen_width, screen_height, 0.0f, 1.0f, -1.0f));
    program.setModel(glm::mat4(1.f));
    program.setView(glm::mat4(1.f));
    program.setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
    program.updateModel();
    program.updateView();
    program.updateProjection();
    program.updateColor();
    program.setTexture(0);
}

void RendererSystem::setScale(GLfloat scale)
{
    scale_factor = scale;
}

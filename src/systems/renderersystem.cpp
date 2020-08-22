#include "systems/renderersystem.hpp"
#include <algorithm>
#include "components/levelcomponent.h"
#include "components/spritecomponent.hpp"
#include "renderer.h"

void RendererSystem::update(size_t delta)
{
    auto levelEntities = getEntitiesByTag<LevelComponent>();
    program.setTextureRendering(false);
    for (auto& [key, en]: levelEntities) {
        Renderer::render(program, en->getComponent<LevelComponent>()->points,
                         en->getComponent<LevelComponent>()->stars);
    }

    auto sprites = getEntitiesByTag<SpriteComponent>();
    program.setTextureRendering(true);
    for (auto& [key, en]: sprites) {
        Renderer::render(program, *en->getComponent<SpriteComponent>()->sprite,
                               en->getComponent<PositionComponent>()->x,
                               en->getComponent<PositionComponent>()->y,
                               en->getComponent<PositionComponent>()->angle);
    }

    auto textComponents = getEntitiesByTag<TextComponent>();
    for (auto& [key, en]: textComponents) {
        Renderer::render(program, *en->getComponent<TextComponent>()->texture,
                                en->getComponent<PositionComponent>()->x,
                                en->getComponent<PositionComponent>()->y,
                                en->getComponent<PositionComponent>()->angle);
    }
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

void RendererSystem::move_from_camera()
{
    for (auto& en: movables) {
        auto pos = en.getComponent<PositionComponent>();
        if (pos != nullptr) {
            pos->x -= camera.getX();
        } else {
            auto level = en.getComponent<LevelComponent>();
            for (auto & point : level->points)
                point.x -= camera.getX();
            for (auto& star : level->stars)
                star.x -= camera.getX();
        }
    }
}

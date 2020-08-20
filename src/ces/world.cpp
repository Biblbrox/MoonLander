#include "../include/ces/world.hpp"
#include "../include/ces/entity.hpp"

#include <memory>
#include <components/positioncomponent.hpp>
#include <components/displaycomponent.hpp>
#include <components/spritecomponent.hpp>
#include <components/velocitycomponent.hpp>
#include <components/levelcomponent.h>
#include <systems/renderersystem.hpp>
#include <systems/movementsystem.hpp>
#include <components/textcomponent.h>
#include <SDL_ttf.h>
#include <systems/keyboardsystem.h>
#include <systems/animationsystem.hpp>

const int SHIP_WIDTH = 20;
const int SHIP_HEIGHT = 21;

const GLfloat gravity_force = 0.5f;
const GLfloat weight = 40.f;
const GLfloat engine_force = 1.f;
const GLfloat rot_step = 0.004f;

Entity& World::createEntity()
{
    Entity ent;
    ent.setWorld(std::shared_ptr<World>(this));
    entities.push_back(ent);
    return entities.back();
}

std::vector<Entity>& World::getEntities()
{
    return entities;
}

void World::update(size_t delta)
{
    for (auto& system: systems) {
        system.second->update(delta);
    }
}

void World::init()
{
    GLuint screen_height = Utils::getScreenHeight<GLuint>();
    GLuint screen_width = Utils::getScreenWidth<GLuint>();

    // Ship entity
    Entity& ship = createEntity();
    ship.addComponent<PositionComponent>();
    ship.addComponent<DisplayComponent>();
    ship.addComponent<SpriteComponent>();
    ship.addComponent<VelocityComponent>();
    ship.addComponent<KeyboardComponent>();
    ship.addComponent<AnimationComponent>();

    auto spriteComponent = ship.getComponent<SpriteComponent>();
    spriteComponent->sprite = std::make_shared<Sprite>(Utils::getResourcePath("lunar_lander_bw.png"));
    spriteComponent->sprite->addClipSprite({.x = 0,  .y = 32, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    spriteComponent->sprite->addClipSprite({.x = 20, .y = 32, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    spriteComponent->sprite->addClipSprite({.x = 40, .y = 32, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    spriteComponent->sprite->setIdx(0);
    spriteComponent->sprite->generateDataBuffer();

    auto posComponent = ship.getComponent<PositionComponent>();
    posComponent->x = 40;
    posComponent->y = 10;

    auto velComponent = ship.getComponent<VelocityComponent>();
    velComponent->x = 0.f;
    velComponent->y = 0.f;
    velComponent->vel_angle = 0.f;
    velComponent->is_moving = false;

    auto animationShip = ship.getComponent<AnimationComponent>();
    animationShip->cur_state = 0;

    auto keyboardComponent = ship.getComponent<KeyboardComponent>();
    keyboardComponent->event_handler = [velComponent, posComponent, animationShip](const Uint8* state){
        if (state[SDL_SCANCODE_UP]) {
            if (!velComponent->is_moving)
                velComponent->is_moving = true;

            velComponent->y += -engine_force / weight *
                           std::sin(posComponent->angle + glm::half_pi<GLfloat>());
            velComponent->x += -engine_force / weight *
                           std::cos(posComponent->angle + glm::half_pi<GLfloat>());
            animationShip->cur_state = (SDL_GetTicks() / 100) % 2 + 1;
        } else {
            animationShip->cur_state = 0;
            if (velComponent->is_moving)
                velComponent->is_moving = false;
        }

        if (state[SDL_SCANCODE_LEFT] /* && !ship.isLanded() */)
            velComponent->vel_angle -= rot_step;

        if (state[SDL_SCANCODE_RIGHT] /* && !ship.isLanded() */)
            velComponent->vel_angle += rot_step;
//
//        if (level.hasCollision({.x = ship.getX(), .y = ship.getY(),
//                                      .w = SHIP_WIDTH,  .h = SHIP_HEIGHT}, ship.getAngle())) {
//            if (!ship.isLanded()) {
//                ship.setVel({.x = 0, .y = 0});
//                ship.setVelRot(0.f);
//            }
//            if (!ship.isLanded())
//                ship.turnLanded();
//
//            if (ship.enginesOn())
//                ship.addCoords({.x = ship.getVelX(), .y = ship.getVelY()});
//        } else {
//            if (ship.isLanded())
//                ship.turnLanded();
//            ship.addCoords({.x = ship.getVelX(), .y = ship.getVelY()});
//            ship.rotate(ship.getVelRot());
//            ship.addVelY(gravity_force / weight);
//        }
    };

    // Level entity
    Entity& level = createEntity();
    level.addComponent<DisplayComponent>();
    level.addComponent<LevelComponent>();

    // Fps entity
    Entity& fpsText = createEntity();
    fpsText.addComponent<TextComponent>();
    fpsText.addComponent<PositionComponent>();

    auto fspTexture = fpsText.getComponent<TextComponent>();
    fspTexture->texture = std::make_shared<TextTexture>("FPS: 60");

    auto fpsPos = fpsText.getComponent<PositionComponent>();
    fpsPos->x = screen_width - screen_width / 4.5f;
    fpsPos->y = screen_height / 15.f;

    createSystem<RendererSystem>();
    createSystem<MovementSystem>();
    createSystem<KeyboardSystem>();
    createSystem<AnimationSystem>();
}

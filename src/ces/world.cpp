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
#include <systems/collisionsystem.hpp>
#include <sstream>
#include <iomanip>
#include <systems/physics_system.h>

const int SHIP_WIDTH = 20;
const int SHIP_HEIGHT = 21;

const GLfloat gravity_force = 0.5f;
const GLfloat weight = 40.f;
const GLfloat engine_force = 1.f;
const GLfloat rot_step = 0.004f;

Entity& World::createEntity(std::string name)
{
    std::shared_ptr<Entity> ent = std::make_shared<Entity>();
    ent->setWorld(std::shared_ptr<World>(this));
    entities.insert({name, ent});
    return *entities[name];
}

std::unordered_map<std::string, std::shared_ptr<Entity>>& World::getEntities()
{
    return entities;
}

void World::update_ship()
{
    Entity ship = *entities["ship"];
    auto shipPos = ship.getComponent<PositionComponent>();
    auto shipVel = ship.getComponent<VelocityComponent>();
    if (shipPos->x >= screen_width - screen_width / 4.f) {
        camera.translate(shipVel->x, 0.f);
        move_from_camera();
    }

    if (shipPos->x <= screen_width / 4.f) {
        camera.translate(shipVel->x, 0.f);
        move_from_camera();
    }

    auto colShip = ship.getComponent<CollisionComponent>();

    if (colShip->has_collision) {
        shipVel->x = 0.f;
        shipVel->y = 0.f;
        shipVel->vel_angle = 0.f;
    }
}

void World::update_text()
{
    Entity fpsEntity = *entities["fpsText"];
    Entity velxEntity = *entities["velxText"];
    Entity velyEntity = *entities["velyText"];
    Entity shipEntity = *entities["ship"];
    auto shipVel = shipEntity.getComponent<VelocityComponent>();

    auto textFps = fpsEntity.getComponent<TextComponent>();
    auto textVelX = velxEntity.getComponent<TextComponent>();
    auto textVelY = velyEntity.getComponent<TextComponent>();

    std::stringstream fpsText;
    std::stringstream velxText;
    std::stringstream velyText;

    fpsText << "FPS: " << std::floor(fps.get_fps());
    velxText << std::fixed << std::setprecision(0) << "Horizontal speed x: " << shipVel->x * 60.f;
    velyText << std::fixed <<  std::setprecision(0) << "Vertical speed y: " << -shipVel->y * 60.f << std::fixed;

    textFps->texture->setText(fpsText.str());
    textVelX->texture->setText(velxText.str());
    textVelY->texture->setText(velyText.str());
}

void World::update(size_t delta)
{
    fps.update();
    update_ship();
    update_text();

    for (auto& system: systems) {
        system.second->update(delta);
    }
}

void World::init()
{
    screen_height = Utils::getScreenHeight<GLuint>();
    screen_width = Utils::getScreenWidth<GLuint>();

    // Ship entity
    Entity& ship = createEntity("ship");
    ship.addComponent<PositionComponent>();
    ship.addComponent<DisplayComponent>();
    ship.addComponent<SpriteComponent>();
    ship.addComponent<VelocityComponent>();
    ship.addComponent<KeyboardComponent>();
    ship.addComponent<AnimationComponent>();
    ship.addComponent<CollisionComponent>();

    auto spriteComponent = ship.getComponent<SpriteComponent>();
    spriteComponent->sprite = std::make_shared<Sprite>(Utils::getResourcePath("lunar_lander_bw.png"));
    spriteComponent->sprite->addClipSprite({.x = 0,  .y = 32, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    spriteComponent->sprite->addClipSprite({.x = 20, .y = 32, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    spriteComponent->sprite->addClipSprite({.x = 40, .y = 32, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    spriteComponent->sprite->generateDataBuffer();

    auto shipPos = ship.getComponent<PositionComponent>();
    shipPos->x = screen_width / 2.f;
    shipPos->y = 10.f;

    auto shipVel = ship.getComponent<VelocityComponent>();

    auto shipAnim = ship.getComponent<AnimationComponent>();
    // Ship reference invalidate

    auto keyboardComponent = ship.getComponent<KeyboardComponent>();
    keyboardComponent->event_handler = [shipVel, shipPos, shipAnim](const Uint8* state){
        if (state[SDL_SCANCODE_UP]) {
            shipVel->y += -engine_force / weight *
                           std::sin(shipPos->angle + glm::half_pi<GLfloat>());
            shipVel->x += -engine_force / weight *
                           std::cos(shipPos->angle + glm::half_pi<GLfloat>());
            shipAnim->cur_state = (SDL_GetTicks() / 100) % 2 + 1;
        } else {
            shipAnim->cur_state = 0;
        }

        if (state[SDL_SCANCODE_LEFT])
            shipVel->vel_angle -= rot_step;

        if (state[SDL_SCANCODE_RIGHT])
            shipVel->vel_angle += rot_step;
    };

    // Level entity
    Entity& level = createEntity("level");
    level.addComponent<DisplayComponent>();
    level.addComponent<LevelComponent>();
    level.addComponent<CollisionComponent>();

    LevelGenerator generator;
    auto levelComponent = level.getComponent<LevelComponent>();
    levelComponent->points = generator.generate_lines(0);
    levelComponent->stars = generator.generate_stars();
    // Level reference invalidate

    // Fps entity
    Entity& fpsText = createEntity("fpsText");
    fpsText.addComponent<TextComponent>();
    fpsText.addComponent<PositionComponent>();

    auto fspTexture = fpsText.getComponent<TextComponent>();
    fspTexture->texture = std::make_shared<TextTexture>("FPS: 000");

    auto fpsPos = fpsText.getComponent<PositionComponent>();
    fpsPos->x = screen_width - screen_width / 4.2f;
    fpsPos->y = screen_height / 15.f;

    // Velocity x entity
    Entity& velxText = createEntity("velxText");
    velxText.addComponent<TextComponent>();
    velxText.addComponent<PositionComponent>();

    auto velxTexture = velxText.getComponent<TextComponent>();
    velxTexture->texture = std::make_shared<TextTexture>("Horizontal speed x: -000.000");

    auto velxPos = velxText.getComponent<PositionComponent>();
    velxPos->x = screen_width - screen_width / 4.2f;
    velxPos->y = screen_height / 10.f;

    // Velocity y entity
    Entity& velyText = createEntity("velyText");
    velyText.addComponent<TextComponent>();
    velyText.addComponent<PositionComponent>();

    auto velyTexture = velyText.getComponent<TextComponent>();
    velyTexture->texture = std::make_shared<TextTexture>("Vertical speed y: -000.000");

    auto velyPos = velyText.getComponent<PositionComponent>();
    velyPos->x = screen_width - screen_width / 4.2f;
    velyPos->y = screen_height / 8.f;

    createSystem<RendererSystem>();
    createSystem<MovementSystem>();
    createSystem<KeyboardSystem>();
    createSystem<AnimationSystem>();
    createSystem<CollisionSystem>();
    createSystem<PhysicsSystem>();

    non_static.push_back(*entities["level"]);
    non_static.push_back(*entities["ship"]);
}

void World::move_from_camera()
{
    for (auto& en: non_static) {
        auto pos = en.getComponent<PositionComponent>();
        if (pos != nullptr) {
            pos->x -= camera.deltaX();
        } else {
            auto level = en.getComponent<LevelComponent>();
            for (auto & point : level->points)
                point.x -= camera.deltaX();
            for (auto& star : level->stars)
                star.x -= camera.deltaX();
        }
    }
}

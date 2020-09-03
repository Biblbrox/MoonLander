#include "../include/ces/world.hpp"

#include <memory>
#include <components/positioncomponent.hpp>
#include <components/displaycomponent.hpp>
#include <components/spritecomponent.hpp>
#include <components/velocitycomponent.hpp>
#include <components/levelcomponent.hpp>
#include <systems/renderersystem.hpp>
#include <systems/movementsystem.hpp>
#include <components/textcomponent.hpp>
#include <SDL_ttf.h>
#include <systems/keyboardsystem.hpp>
#include <systems/animationsystem.hpp>
#include <systems/collisionsystem.hpp>
#include <systems/physics_system.hpp>
#include <particle/particleengine.h>
#include <systems/particlerendersystem.h>

using utils::log::Logger;
using boost::format;

const int SHIP_WIDTH = 20;
const int SHIP_HEIGHT = 21;

Entity& World::createEntity(const std::string& name)
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

void World::rescale_world()
{
    frame_width = scaled ? screen_width : (screen_width / scale_factor);
    frame_height = scaled ? screen_height : (screen_height / scale_factor);
    auto ship = entities["ship"];
    auto level = entities["level"];
    auto renderSystem = std::dynamic_pointer_cast<RendererSystem>(systems[type_id<RendererSystem>()]);
    auto shipPos = ship->getComponent<PositionComponent>();

    level->getComponent<LevelComponent>()->scale_factor = scaled ? 1.f : scale_factor;
    auto scaled_entities = renderSystem->getEntitiesByTag<PositionComponent>();
    for (auto& [key, en]: scaled_entities) {
        auto pos = en->getComponent<PositionComponent>();
        if (pos->scallable)
            pos->scale_factor = scaled ? 1.f : scale_factor;
    }
    if (scaled)
        camera.lookAt(shipPos->x + camera.getX() - frame_width / 2.f,
                      shipPos->y + camera.getY() - frame_width / 5.f);
    else
        camera.lookAt(shipPos->x + camera.getX() - frame_width / 2.f,
                      shipPos->y + camera.getY() - frame_height / 4.f);

    scaled = !scaled;
    move_from_camera();
}

void World::update_ship()
{
    auto ship = entities["ship"];
    auto shipPos = ship->getComponent<PositionComponent>();
    auto shipVel = ship->getComponent<VelocityComponent>();

    auto renderSystem = std::dynamic_pointer_cast<RendererSystem>(
            systems[type_id<RendererSystem>()]
    );
    auto levelComp = entities["level"]->getComponent<LevelComponent>();
    const GLfloat shipHeight = (utils::physics::ship_altitude(levelComp->points, shipPos->x, shipPos->y));
    const GLfloat alt_threshold = 100.f; // Threshold when world will be scaled
    if ((shipHeight < alt_threshold && !scaled) // Need to increase scale
        || (shipHeight >= alt_threshold && scaled)) { // Need to decrease scale
        rescale_world();
    }

    if ((shipPos->x >= frame_width - frame_width / 4.f)
        || (shipPos->x < frame_width / 4.f)) { // Horizontal edges
        camera.translate(shipVel->x, 0.f);
        move_from_camera();
    }

    if ((shipPos->y >= frame_height - frame_height / 4.f)
               || (shipPos->y < frame_height / 4.f)) { // Vertical edges
        camera.translate(0.f, shipVel->y);
        move_from_camera();
    }

    auto colShip = ship->getComponent<CollisionComponent>();

    if (colShip->has_collision) {
        utils::Rect shipClip = {0, 32, SHIP_WIDTH, SHIP_HEIGHT};
        std::vector<utils::Position> coords;
        std::vector<utils::Position> vel;
        coords.resize(4);
        vel.resize(4);
        std::fill(coords.begin(), coords.end(),
                  utils::Position{shipPos->x, shipPos->y, shipPos->angle});
        std::fill(vel.begin(), vel.end(),
                  utils::Position{shipVel->x, shipVel->y, shipVel->vel_angle});

        utils::RandomUniform rand;
        std::generate(vel.begin(), vel.end(), [&rand, shipVel](){
            utils::Position res;
            const GLfloat deviation = 1.5f;
            const GLfloat scale_vel = 20.f;
            const GLfloat scale_vel_rot = 30.f;
            res.x = rand.generaten<GLfloat>(shipVel->x / scale_vel, deviation);
            res.y = rand.generaten<GLfloat>(shipVel->y / scale_vel, deviation);
            res.angle = rand.generaten<GLfloat>(shipVel->vel_angle / scale_vel_rot, deviation);
            return res;
        });

        auto particle = ParticleEngine::generateParticleFromTexture(
                utils::getResourcePath("lunar_lander_bw.png"),
                generate_clips(shipClip, 2, 2),
                coords,
                vel, 10000.f);

        entities.insert({"ship particle", particle});

        Logger::write(utils::program_log_file_name(), utils::log::Category::INFO,
                (format("\nX: %1%\nY: %2%\n") % shipPos->x % shipPos->y).str());

        ship->kill();
    }
}

void World::update_text()
{
    auto fpsEntity = entities["fpsText"];
    auto velxEntity = entities["velxText"];
    auto velyEntity = entities["velyText"];

    auto textFps = fpsEntity->getComponent<TextComponent>();
    auto textVelX = velxEntity->getComponent<TextComponent>();
    auto textVelY = velyEntity->getComponent<TextComponent>();

    if (entities.contains("ship")) {
        auto shipEntity = entities["ship"];

        auto shipVel = shipEntity->getComponent<VelocityComponent>();

        textFps->texture->setText(
                (boost::format("FPS: %+3d") % fps.get_fps()).str());
        textVelX->texture->setText((boost::format("Horizontal speed: %3d") %
                                    std::floor(shipVel->x * 60.f)).str());
        textVelY->texture->setText((boost::format("Vertical speed: %3d") %
                                    std::floor(-shipVel->y * 60.f)).str());
    } else {
        textFps->texture->setText(
                (boost::format("FPS: %+3d") % fps.get_fps()).str());
        textVelX->texture->setText((boost::format("Horizontal speed: %3d") %
                                    std::floor(0.f)).str());
        textVelY->texture->setText((boost::format("Vertical speed: %3d") %
                                    std::floor(0.f)).str());
    }
}

void World::update(size_t delta)
{
    fps.update();
    if (entities.count("ship") != 0)
        update_ship();

    update_text();

    for (auto& system: systems) {
        system.second->update(delta);
    }

    filter_entities();
}

void World::init()
{
    screen_width = utils::getScreenWidth<GLuint>();
    screen_height = utils::getScreenHeight<GLuint>();
    frame_width = screen_width;
    frame_height = screen_height;

    // Level entity
    Entity& level = createEntity("level");
    level.addComponent<DisplayComponent>();
    level.addComponent<LevelComponent>();
    level.addComponent<CollisionComponent>();
    level.activate();

    LevelGenerator generator;
    auto levelComponent = level.getComponent<LevelComponent>();
    levelComponent->points = generator.generate_lines(0);
    GLfloat points_right_x =
            levelComponent->points[levelComponent->points.size() - 1].x;
    levelComponent->stars = generator.generate_stars(0, points_right_x);
    generator.extendToLeft(levelComponent->points, levelComponent->stars);
    generator.extendToRight(levelComponent->points, levelComponent->stars);
    // Level reference invalidate

    // Ship entity
    Entity& ship = createEntity("ship");
    ship.addComponent<PositionComponent>();
    ship.addComponent<DisplayComponent>();
    ship.addComponent<SpriteComponent>();
    ship.addComponent<VelocityComponent>();
    ship.addComponent<KeyboardComponent>();
    ship.addComponent<AnimationComponent>();
    ship.addComponent<CollisionComponent>();
    ship.activate();

    auto shipSprite = ship.getComponent<SpriteComponent>();
    shipSprite->sprite = std::make_shared<Sprite>(
            utils::getResourcePath("lunar_lander_bw.png"));
    shipSprite->sprite->addClipSprite
            ({.x = 0,  .y = 32, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    shipSprite->sprite->addClipSprite
            ({.x = 20, .y = 32, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    shipSprite->sprite->addClipSprite
            ({.x = 40, .y = 32, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    shipSprite->sprite->generateDataBuffer();

    auto shipPos = ship.getComponent<PositionComponent>();
    shipPos->x = screen_width / 2.f;
    shipPos->y = utils::physics::alt_from_surface(entities["level"]->getComponent<LevelComponent>()->points,
                                                  shipPos->x, screen_height / 4.f);

    auto shipVel = ship.getComponent<VelocityComponent>();

    auto shipAnim = ship.getComponent<AnimationComponent>();
    // Ship reference invalidate

    Entity& earth = createEntity("earth");
    earth.addComponent<PositionComponent>();
    earth.addComponent<DisplayComponent>();
    earth.addComponent<SpriteComponent>();
    earth.activate();

    auto earthSprite = earth.getComponent<SpriteComponent>();
    earthSprite->sprite = std::make_shared<Sprite>(utils::getResourcePath("lunar_lander_bw.png"));
    earthSprite->sprite->addClipSprite({.x = 200, .y = 77, .w = 40, .h = 33});
    earthSprite->sprite->generateDataBuffer();

    auto earthPos = earth.getComponent<PositionComponent>();
    earthPos->x = screen_width / 10.f;
    earthPos->y = screen_height / 10.f;
    earthPos->scallable = false;

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

    // Fps entity
    Entity& fpsText = createEntity("fpsText");
    fpsText.addComponent<TextComponent>();
    fpsText.addComponent<PositionComponent>();
    fpsText.activate();

    auto fspTexture = fpsText.getComponent<TextComponent>();
    fspTexture->texture = std::make_shared<TextTexture>("FPS: 000");

    auto fpsPos = fpsText.getComponent<PositionComponent>();
    fpsPos->x = screen_width - screen_width / 4.2f;
    fpsPos->y = screen_height / 15.f;
    fpsPos->scallable = false;

    // Velocity x entity
    Entity& velxText = createEntity("velxText");
    velxText.addComponent<TextComponent>();
    velxText.addComponent<PositionComponent>();
    velxText.activate();

    auto velxTexture = velxText.getComponent<TextComponent>();
    velxTexture->texture = std::make_shared<TextTexture>("Horizontal speed: -000.000");

    auto velxPos = velxText.getComponent<PositionComponent>();
    velxPos->x = screen_width - screen_width / 4.2f;
    velxPos->y = screen_height / 10.f;
    velxPos->scallable = false;

    // Velocity y entity
    Entity& velyText = createEntity("velyText");
    velyText.addComponent<TextComponent>();
    velyText.addComponent<PositionComponent>();
    velyText.activate();

    auto velyTexture = velyText.getComponent<TextComponent>();
    velyTexture->texture = std::make_shared<TextTexture>("Vertical speed: -000.000");

    auto velyPos = velyText.getComponent<PositionComponent>();
    velyPos->x = screen_width - screen_width / 4.2f;
    velyPos->y = screen_height / 8.f;
    velyPos->scallable = false;

    createSystem<RendererSystem>();
    createSystem<MovementSystem>();
    createSystem<KeyboardSystem>();
    createSystem<AnimationSystem>();
    createSystem<CollisionSystem>();
    createSystem<PhysicsSystem>();
    createSystem<ParticleRenderSystem>();

    non_static.push_back(entities["level"]);
    non_static.push_back(entities["ship"]);
}

void World::move_from_camera()
{
    for (const auto& en: non_static) {
        auto pos = en->getComponent<PositionComponent>();
        if (pos != nullptr) {
            pos->x -= camera.deltaX();
            pos->y -= camera.deltaY();
        } else {
            auto level = en->getComponent<LevelComponent>();
            for (auto & point : level->points) {
                point.x -= camera.deltaX();
                point.y -= camera.deltaY();
            }
            for (auto& star : level->stars) {
                star.x -= camera.deltaX();
                star.y -= camera.deltaY();
            }
        }
    }
}

std::vector<utils::Rect>
World::generate_clips(utils::Rect clip, size_t num_x, size_t num_y)
{
    GLfloat part_width = clip.w / num_x;
    GLfloat part_height = clip.h / num_y;

    GLfloat x;
    GLfloat y;

    std::vector<utils::Rect> clips;
    clips.reserve(num_x * num_y);
    for (y = clip.y; y < (clip.y + clip.h); y += part_height) {
        for (x = clip.x; x < (clip.x + clip.w); x += part_width) {
            clips.push_back({x, y, part_width, part_height});
        }
    }

    return clips;
}

void World::filter_entities()
{
    for (auto it = entities.begin(); it != entities.end();) {
        if (!it->second->isActivate())
            it = entities.erase(it);
        else
            ++it;
    }
}


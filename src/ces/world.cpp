#include "../../include/base.h"

#include "../include/ces/world.hpp"

#include <memory>
#include <components/positioncomponent.hpp>
#include <components/spritecomponent.hpp>
#include <components/velocitycomponent.hpp>
#include <components/levelcomponent.hpp>
#include <systems/renderersystem.hpp>
#include <systems/movementsystem.hpp>
#include <components/textcomponent.hpp>
#include <systems/keyboardsystem.hpp>
#include <systems/animationsystem.hpp>
#include <systems/collisionsystem.hpp>
#include <systems/physics_system.hpp>
#include <particle/particleengine.hpp>
#include <systems/particlerendersystem.hpp>
#include <utils/random.h>
#include <components/lifetimecomponent.h>
#include <game.hpp>

using utils::log::Logger;
using utils::getResourcePath;
using boost::format;
using utils::physics::altitude;
using std::floor;
using std::vector;
using std::make_shared;
using std::sin;
using std::cos;
using glm::half_pi;

const int SHIP_WIDTH = 20;
const int SHIP_HEIGHT = 21;

const int engine_channel = 0;
const int crash_sound_channel = 1;

const int engine_idx = 0;
const int crash_idx = 1;

const char* msgFont = "kenvector_future2.ttf";

Entity& World::createEntity(const std::string& name)
{
    std::shared_ptr<Entity> ent = make_shared<Entity>();
    ent->setWorld(this);
    m_entities.insert({name, ent});
    return *m_entities[name];
}

std::unordered_map<std::string, std::shared_ptr<Entity>>& World::getEntities()
{
    return m_entities;
}

void World::rescale_world()
{
    m_frameWidth = m_scaled ? m_screenWidth : (m_screenWidth / m_scaleFactor);
    m_frameHeight = m_scaled ? m_screenHeight : (m_screenHeight / m_scaleFactor);
    auto ship = m_entities["ship"];
    auto level = m_entities["level"];
    size_t idx = type_id<RendererSystem>();
    auto renderSystem = std::dynamic_pointer_cast<RendererSystem>(m_systems[idx]);
    auto shipPos = ship->getComponent<PositionComponent>();

    level->getComponent<LevelComponent>()->scale_factor = m_scaled ?
                                                          1.f : m_scaleFactor;
    auto scaled_entities = renderSystem->getEntitiesByTag<PositionComponent>();
    for (auto& [key, en]: scaled_entities) {
        auto pos = en->getComponent<PositionComponent>();
        if (pos->scallable)
            pos->scale_factor = m_scaled ? 1.f : m_scaleFactor;
    }
    if (m_scaled)
        m_camera.lookAt(shipPos->x + m_camera.getX() - m_frameWidth / 2.f,
                        shipPos->y + m_camera.getY() - m_frameWidth / 5.f);
    else
        m_camera.lookAt(shipPos->x + m_camera.getX() - m_frameWidth / 2.f,
                        shipPos->y + m_camera.getY() - m_frameHeight / 4.f);

    m_scaled = !m_scaled;
    move_from_camera();
}

void World::update_ship()
{
    using utils::physics::altitude;
    using utils::Position;

    auto ship = m_entities["ship"];
    auto shipPos = ship->getComponent<PositionComponent>();
    auto shipVel = ship->getComponent<VelocityComponent>();

    auto renderSystem = std::dynamic_pointer_cast<RendererSystem>(
            m_systems[type_id<RendererSystem>()]
    );
    auto levelComp = m_entities["level"]->getComponent<LevelComponent>();
    const GLfloat shipHeight = altitude(levelComp->points, shipPos->x, shipPos->y);
    const GLfloat alt_threshold = 100.f; // Threshold when world will be scaled
    if ((shipHeight < alt_threshold && !m_scaled) // Need to increase scale
        || (shipHeight >= alt_threshold && m_scaled)) { // Need to decrease scale
        rescale_world();
    }

    if ((shipPos->x >= m_frameWidth - m_frameWidth / 4.f)
        || (shipPos->x < m_frameWidth / 4.f)) { // Horizontal edges
        m_camera.translate(shipVel->x, 0.f);
        move_from_camera();
    }

    if ((shipPos->y >= m_frameHeight - m_frameHeight / 4.f)
               || (shipPos->y < m_frameHeight / 4.f)) { // Vertical edges
        m_camera.translate(0.f, shipVel->y);
        move_from_camera();
    }

    auto colShip = ship->getComponent<CollisionComponent>();
    if (colShip->has_collision) {
        const vector<vec2>& platforms = levelComp->platforms;
        bool landed = false;
        assert(platforms.size() % 2 == 0);
        GLfloat angle = shipPos->angle - shipPos->angle / (2 * glm::pi<GLfloat>());
        for (size_t i = 0; i < platforms.size(); i += 2) {
            if (shipPos->x >= platforms[i].x
                && shipPos->x <= platforms[i + 1].x
                && angle >= -glm::pi<GLfloat>() / 6.f
                && angle <= glm::pi<GLfloat>() / 6.f
                && std::abs(shipVel->y * 60.f) <= 20) {
                shipVel->x = 0;
                shipVel->y = 0;
                shipVel->angle = 0;
                landed = true;
                Game::getInstance()->setState(GameStates::WIN);
                break;
            }
        }

        if (!landed) {
            utils::Rect shipClip{0, 32, SHIP_WIDTH, SHIP_HEIGHT};
            vector<Position> coords(16, {shipPos->x, shipPos->y, shipPos->angle});
            vector<Position> vel(16, {shipVel->x, shipVel->y, shipVel->angle});

            utils::Random rand;
            // Init velocities of particles
            std::generate(vel.begin(), vel.end(), [&rand, shipVel]() {
                const GLfloat deviation = 1.5f;
                const GLfloat scale_vel = 20.f;
                const GLfloat scale_vel_rot = 30.f;
                return utils::Position {
                        rand.generaten(shipVel->x / scale_vel, deviation),
                        rand.generaten(shipVel->y / scale_vel, deviation),
                        rand.generaten(shipVel->angle / scale_vel_rot, deviation)
                };
            });

            auto particle = ParticleEngine::generateFromTexture(
                    utils::getResourcePath("lunar_lander_bw.png"),
                    generate_clips(shipClip, 4, 4), coords, vel, 10000.f);

            particle->setWorld(this);
            m_entities.insert({"ship particle", particle});

            ship->kill();

            if (!m_audio.isChannelPlaying(crash_sound_channel))
                m_audio.playChunk(crash_sound_channel, crash_idx, 0, false);
            Game::getInstance()->setState(GameStates::FAIL);
        }

        m_audio.haltChannel(engine_channel, true);
    }
}

void World::update_text()
{
    using boost::format;

    if constexpr (debug) {
        auto fpsEntity = m_entities["fpsText"];
        auto textFps = fpsEntity->getComponent<TextComponent>();
        textFps->texture->setText((format("FPS: %+3d") % m_fps.get_fps()).str());
    }

    auto velxEntity = m_entities["velxText"];
    auto velyEntity = m_entities["velyText"];
    auto altEntity = m_entities["altitude"];
    auto fuelEntity = m_entities["fuel"];

    auto textVelX = velxEntity->getComponent<TextComponent>();
    auto textVelY = velyEntity->getComponent<TextComponent>();
    auto textAlt = altEntity->getComponent<TextComponent>();
    auto textFuel = fuelEntity->getComponent<TextComponent>();

    auto game = Game::getInstance();
    if (game->getState() == GameStates::NORMAL
        || game->getState() == GameStates::WIN) {
        auto shipEntity = m_entities["ship"];
        auto points = m_entities["level"]->getComponent<LevelComponent>()->points;

        auto shipVel = shipEntity->getComponent<VelocityComponent>();
        auto shipPos = shipEntity->getComponent<PositionComponent>();
        auto fuel = shipEntity->getComponent<LifeTimeComponent>();

        textVelX->texture->setText((format("Horizontal speed: %3d") %
                                    floor(shipVel->x * 60.f)).str());
        textVelY->texture->setText((format("Vertical speed: %3d") %
                                    floor(-shipVel->y * 60.f)).str());
        textAlt->texture->setText((format("Altitude: %3d") %
                                   floor(altitude(points, shipPos->x, shipPos->y)
                                         - SHIP_HEIGHT)).str());
        textFuel->texture->setText((format("Fuel: %3d") % fuel->time).str());

        if (game->getState() == GameStates::WIN
            && m_entities.count("winText") == 0) {
            Entity& winText = createEntity("winText");
            winText.addComponents<TextComponent, PositionComponent>();
            winText.activate();

            auto winTextTexture = winText.getComponent<TextComponent>();
            TTF_Font* font = TTF_OpenFont(getResourcePath(msgFont).c_str(), 14);
            if (!font) {
                Logger::write(utils::program_log_file_name(), Category::FILE_ERROR,
                              (format("Unable to load font %s\n") % msgFont).str());
                std::abort();
            }
            winTextTexture->texture =
                    make_shared<TextTexture>("You win\nScore is 000.000\n"
                                             "To play again press Enter",
                                             font);

            auto winTextPos = winText.getComponent<PositionComponent>();
            winTextPos->x = m_screenWidth / 2.f
                            - winTextTexture->texture->getWidth() / 2.f;
            winTextPos->y = m_screenHeight / 2.f
                            - winTextTexture->texture->getHeight() / 2.f;
            winTextPos->scallable = false;
        }
    } else if (game->getState() == GameStates::FAIL
               && m_entities.count("failText") == 0) { // Fail case
        textVelX->texture->setText((format("Horizontal speed: %3d") % 0).str());
        textVelY->texture->setText((format("Vertical speed: %3d") % 0).str());

        Entity& failText = createEntity("failText");
        failText.addComponents<TextComponent, PositionComponent>();
        failText.activate();

        auto failTextTexture = failText.getComponent<TextComponent>();
        TTF_Font* font = TTF_OpenFont(getResourcePath(msgFont).c_str(), 14);
        if (font == nullptr) {
            Logger::write(utils::program_log_file_name(), Category::FILE_ERROR,
                          (format("Unable to load font %s\n") % msgFont).str());
            std::abort();
        }
        failTextTexture->texture =
                make_shared<TextTexture>("You fail\n"
                                         "To play again press Enter", font);

        auto failTextPos = failText.getComponent<PositionComponent>();
        failTextPos->x = m_screenWidth / 2.f
                         - failTextTexture->texture->getWidth() / 2.f;
        failTextPos->y = m_screenHeight / 2.f
                         - failTextTexture->texture->getHeight() / 2.f;
        failTextPos->scallable = false;
    }
}

void World::update(size_t delta)
{
    if constexpr (debug)
        m_fps.update();

    auto game = Game::getInstance();
    if (game->getState() == GameStates::WIN
        && game->getPrevState() != GameStates::WIN) {
        m_systems[type_id<MovementSystem>()]->stop();
        m_entities["ship"]->removeComponent<KeyboardComponent>();
    }

    if (game->getState() == GameStates::NORMAL
        || game->getState() == GameStates::WIN)
        update_ship();

    update_text();

    filter_entities();
    for (auto &system: m_systems)
        system.second->update(delta);
}

void World::init()
{
    if (!m_wasInit) {
        m_screenWidth = utils::getScreenWidth<GLuint>();
        m_screenHeight = utils::getScreenHeight<GLuint>();
        m_frameWidth = m_screenWidth;
        m_frameHeight = m_screenHeight;

        // Order of initialization is matter
        init_level();
        init_ship();
        init_sprites();
        init_text();
        init_sound();
        init_sound();

        createSystem<RendererSystem>();
        createSystem<MovementSystem>();
        createSystem<KeyboardSystem>();
        createSystem<AnimationSystem>();
        createSystem<CollisionSystem>();
        createSystem<PhysicsSystem>();
        createSystem<ParticleRenderSystem>();

        m_nonStatic.push_back(m_entities["level"]);
        m_nonStatic.push_back(m_entities["ship"]);

        m_wasInit = true;
    } else {
        if (m_entities.count("winText") != 0)
            m_entities.erase("winText");

        if (m_entities.count("failText") != 0)
            m_entities.erase("failText");

        if (m_entities.count("ship") != 0)
            m_entities.erase("ship");

        if (m_entities.count("ship particle") != 0)
            m_entities.erase("ship particle");

        m_camera.lookAt(0, 0);
        //move_from_camera();
        init_ship();
        rescale_world();
        m_scaled = false;
        m_nonStatic[1] = m_entities["ship"];
    }
}

void World::init_sound()
{
    m_audio.addChunk(utils::getResourcePath("engine.wav"));
    m_audio.addChunk(utils::getResourcePath("explosion.wav"));
    m_audio.setFadeOut(300.f);
    m_audio.setFadeIn(400.f);
}

void World::init_sprites()
{
    using namespace utils::physics;

    Entity &earth = createEntity("earth");
    earth.addComponents<PositionComponent, SpriteComponent>();
    earth.activate();

    auto earthSprite = earth.getComponent<SpriteComponent>();
    earthSprite->sprite = make_shared<Sprite>(
            utils::getResourcePath("lunar_lander_bw.png"));
    earthSprite->sprite->addClipSprite({200, 77, 40, 33});
    earthSprite->sprite->generateDataBuffer();

    auto earthPos = earth.getComponent<PositionComponent>();
    earthPos->x = m_screenWidth / 10.f;
    earthPos->y = m_screenHeight / 10.f;
    earthPos->scallable = false;
}

void World::init_text()
{
    if constexpr (debug) {
        // Fps entity
        Entity &fpsText = createEntity("fpsText");
        fpsText.addComponents<TextComponent, PositionComponent>();
        fpsText.activate();

        auto fspTexture = fpsText.getComponent<TextComponent>();
        TTF_Font *font = TTF_OpenFont(getResourcePath(msgFont).c_str(), 14);
        if (!font) {
            Logger::write(utils::program_log_file_name(), Category::FILE_ERROR,
                          (format("Unable to load font %s\n") % msgFont).str());
            std::abort();
        }
        fspTexture->texture = make_shared<TextTexture>("FPS: 000", font);

        auto fpsPos = fpsText.getComponent<PositionComponent>();
        fpsPos->x = m_screenWidth - m_screenWidth / 4.2f;
        fpsPos->y = m_screenHeight / 15.f;
        fpsPos->scallable = false;
    }

    // Velocity x entity
    Entity &velxText = createEntity("velxText");
    velxText.addComponents<TextComponent, PositionComponent>();
    velxText.activate();

    auto velxTexture = velxText.getComponent<TextComponent>();
    TTF_Font *font = TTF_OpenFont(getResourcePath(msgFont).c_str(), 14);
    if (!font) {
        Logger::write(utils::program_log_file_name(), Category::FILE_ERROR,
                      (format("Unable to load font %s\n") % msgFont).str());
        std::abort();
    }
    velxTexture->texture =
            make_shared<TextTexture>("Horizontal speed: -000.000", font);

    auto velxPos = velxText.getComponent<PositionComponent>();
    velxPos->x = m_screenWidth - m_screenWidth / 4.2f;
    velxPos->y = m_screenHeight / 10.f;
    velxPos->scallable = false;

    // Velocity y entity
    Entity &velyText = createEntity("velyText");
    velyText.addComponents<TextComponent, PositionComponent>();
    velyText.activate();

    auto velyTexture = velyText.getComponent<TextComponent>();
    font = TTF_OpenFont(getResourcePath(msgFont).c_str(), 14);
    if (!font) {
        Logger::write(utils::program_log_file_name(), Category::FILE_ERROR,
                      (format("Unable to load font %s\n") % msgFont).str());
        std::abort();
    }
    velyTexture->texture =
            make_shared<TextTexture>("Vertical speed: -000.000", font);

    auto velyPos = velyText.getComponent<PositionComponent>();
    velyPos->x = m_screenWidth - m_screenWidth / 4.2f;
    velyPos->y = m_screenHeight / 8.f;
    velyPos->scallable = false;

    Entity &altitude = createEntity("altitude");
    altitude.addComponents<TextComponent, PositionComponent>();
    altitude.activate();

    auto altTexture = altitude.getComponent<TextComponent>();
    font = TTF_OpenFont(getResourcePath(msgFont).c_str(), 14);
    if (!font) {
        Logger::write(utils::program_log_file_name(), Category::FILE_ERROR,
                      (format("Unable to load font %s\n") % msgFont).str());
        std::abort();
    }
    altTexture->texture = make_shared<TextTexture>("Altitude: -000.000", font);

    auto altPos = altitude.getComponent<PositionComponent>();
    altPos->x = m_screenWidth - m_screenWidth / 4.2f;
    altPos->y = m_screenHeight / 7.f;
    altPos->scallable = false;

    Entity &fuel = createEntity("fuel");
    fuel.addComponents<PositionComponent, TextComponent>();
    fuel.activate();

    auto fuelTexture = fuel.getComponent<TextComponent>();
    font = TTF_OpenFont(getResourcePath(msgFont).c_str(), 14);
    if (!font) {
        Logger::write(utils::program_log_file_name(), Category::FILE_ERROR,
                      (format("Unable to load font %s\n") % msgFont).str());
        std::abort();
    }
    fuelTexture->texture = make_shared<TextTexture>("Fuel: -000.000", font);

    auto fuelPos = fuel.getComponent<PositionComponent>();
    fuelPos->x = m_screenWidth - m_screenWidth / 4.2f;
    fuelPos->y = m_screenHeight / 6.f;
    fuelPos->scallable = false;
}

void World::init_level()
{
    Entity& level = createEntity("level");
    level.addComponents<LevelComponent, CollisionComponent>();
    level.activate();

    LevelGenerator generator;
    auto levelComponent = level.getComponent<LevelComponent>();
    levelComponent->points = generator.generate_lines(
            vec2(0.f, generator.height_min + generator.height_max) / 2.f);
    GLfloat points_right_x = levelComponent->points.end()->x;
    levelComponent->stars = generator.generate_stars(0, points_right_x);
    levelComponent->platforms = generator.platforms;
    generator.extendToLeft(levelComponent->points, levelComponent->stars);
    generator.extendToRight(levelComponent->points, levelComponent->stars);
}

void World::init_ship()
{
    using namespace utils::physics;
    // Ship entity
    Entity &ship = createEntity("ship");
    ship.addComponents<PositionComponent, SpriteComponent, VelocityComponent,
            KeyboardComponent, AnimationComponent, CollisionComponent,
            /*fuel*/ LifeTimeComponent>();
    ship.activate();

    auto shipSprite = ship.getComponent<SpriteComponent>();
    shipSprite->sprite = make_shared<Sprite>(
            utils::getResourcePath("lunar_lander_bw.png"));
    shipSprite->sprite->addClipSprite({0, 32, SHIP_WIDTH, SHIP_HEIGHT});
    shipSprite->sprite->addClipSprite({20, 32, SHIP_WIDTH, SHIP_HEIGHT});
    shipSprite->sprite->addClipSprite({40, 32, SHIP_WIDTH, SHIP_HEIGHT});
    shipSprite->sprite->generateDataBuffer();

    auto shipPos = ship.getComponent<PositionComponent>();
    shipPos->x = m_screenWidth / 2.f;
    shipPos->y = altitude(
            m_entities["level"]->getComponent<LevelComponent>()->points,
            shipPos->x, m_screenHeight / 4.f);

    auto fuel = ship.getComponent<LifeTimeComponent>();
    fuel->time = 1500;

    auto shipVel = ship.getComponent<VelocityComponent>();
    auto shipAnim = ship.getComponent<AnimationComponent>();

    auto keyboardComponent = ship.getComponent<KeyboardComponent>();
    keyboardComponent->event_handler = [shipVel, shipPos, shipAnim, fuel, this]
            (const Uint8 *state) {
        if (state[SDL_SCANCODE_UP] && fuel->time > 0) {
            shipVel->y += -engine_force / weight *
                          sin(shipPos->angle + half_pi<GLfloat>());
            shipVel->x += -engine_force / weight *
                          cos(shipPos->angle + half_pi<GLfloat>());
            shipAnim->cur_state = (SDL_GetTicks() / 100) % 2 + 1;
            if (!m_audio.isChannelPlaying(engine_channel)
                || m_audio.isChannelPaused(engine_channel))
                m_audio.playChunk(engine_channel, engine_idx, -1, true);
            fuel->time -= 1;
        } else {
            shipAnim->cur_state = 0; m_audio.haltChannel(engine_channel, true);
        }

        if (state[SDL_SCANCODE_LEFT])
            shipVel->angle -= rot_step;

        if (state[SDL_SCANCODE_RIGHT])
            shipVel->angle += rot_step;
    };
    // Ship reference invalidate
}



void World::move_from_camera()
{
    for (const auto& en: m_nonStatic) {
        auto pos = en->getComponent<PositionComponent>();
        if (pos != nullptr) {
            pos->x -= m_camera.deltaX();
            pos->y -= m_camera.deltaY();
        } else {
            auto level = en->getComponent<LevelComponent>();
            for (auto& point : level->points) {
                point.x -= m_camera.deltaX();
                point.y -= m_camera.deltaY();
            }
            for (auto& star : level->stars) {
                star.x -= m_camera.deltaX();
                star.y -= m_camera.deltaY();
            }
            for (auto& platform: level->platforms) {
                platform.x -= m_camera.deltaX();
                platform.y -= m_camera.deltaY();
            }
        }
    }
}

void World::filter_entities()
{
    for (auto it = m_entities.begin(); it != m_entities.end();)
        it = !it->second->isActivate() ? m_entities.erase(it) : ++it;
}


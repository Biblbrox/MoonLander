#include <memory>
#include <glm/gtc/constants.hpp>
#include <boost/format.hpp>

#include "../../include/base.h"
#include "../include/ces/world.hpp"
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
#include <exceptions/sdlexception.h>
#include <iostream>

using utils::log::Logger;
using utils::getResourcePath;
using utils::log::program_log_file_name;
using boost::format;
using utils::physics::altitude;
using std::floor;
using std::vector;
using std::make_shared;
using std::sin;
using std::cos;
using glm::half_pi;
using std::find_if;
using glm::pi;
using utils::fix_coords;

const int SHIP_WIDTH = 20;
const int SHIP_HEIGHT = 21;

const int engine_channel = 0;
const int crash_sound_channel = 1;

const int engine_idx = 0;
const int crash_idx = 1;

const char* const msgFont = "kenvector_future2.ttf";
const SDL_Color fontColor = {0xFF, 0xFF, 0xFF, 0xFF};

const GLfloat ship_init_alt = 500;

Entity& World::createEntity(const std::string& name)
{
    std::shared_ptr ent = make_shared<Entity>();
    ent->setWorld(this);
    m_entities.emplace(name, ent);
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

    auto levelEnt = m_entities["level"];
    levelEnt->getComponent<LevelComponent>()->scale_factor = m_scaled ?
                                                          1.f : m_scaleFactor;

    if (m_entities.count("ship") != 0) {
        auto ship = m_entities["ship"];
        size_t idx = type_id<RendererSystem>();
        auto renderSystem = std::dynamic_pointer_cast<RendererSystem>(
                m_systems[idx]);
        auto shipPos = ship->getComponent<PositionComponent>();
        auto scaled_entities = renderSystem->getEntitiesByTag<PositionComponent>();
        for (auto&[key, en]: scaled_entities) {
            auto pos = en->getComponent<PositionComponent>();
            if (pos->scallable)
                pos->scale_factor = m_scaled ? 1.f : m_scaleFactor;
        }

        m_camera.lookAt(shipPos->x + m_camera.getX() - m_frameWidth / 2.f,
                        shipPos->y + m_camera.getY()
                        - m_frameWidth / (m_scaled ? 5.f : 4.f));
    }

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
    GLfloat shipAlt = altitude(levelComp->points, shipPos->x, shipPos->y);
    const GLfloat alt_threshold = 100.f; // Threshold when world will be scaled
    if ((shipAlt < alt_threshold && !m_scaled) // Need to increase scale
        || (shipAlt >= alt_threshold && m_scaled)) { // Need to decrease scale
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
        GLfloat pi2 = 2.f * pi<GLfloat>();
        GLfloat angle = shipPos->angle -
                        pi2 * static_cast<int>(shipPos->angle / pi2);
        if (angle >= -pi<GLfloat>() / 6.f && angle <= pi<GLfloat>() / 6.f
            && std::abs(shipVel->y * 60.f) <= 20) {

            utils::Rect shipSize = ship->getComponent<SpriteComponent>()->sprite->getCurrentClip();
            for (size_t i = 0; i < platforms.size() - 1; i += 2) {
                if (shipPos->x >= platforms[i].x
                    && shipPos->x <= platforms[i + 1].x
                    && shipPos->x + shipSize.w <= platforms[i + 1].x) {
                    shipVel->x = shipVel->y = shipVel->angle = 0;
                    landed = true;
                    setGameState(GameStates::WIN);
                    break;
                }
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
            m_entities.emplace("ship particle", particle);

            ship->kill();

            if (!m_audio.isChannelPlaying(crash_sound_channel))
                m_audio.playChunk(crash_sound_channel, crash_idx, 0, false);

            setGameState(GameStates::FAIL);
        }

        m_audio.haltChannel(engine_channel, true);
    }
}

void World::update_level()
{
    if (getGameState() != GameStates::NORMAL)
        return;

    auto ship = m_entities["ship"];
    const auto shipPos = ship->getComponent<PositionComponent>();

    vec2 shipCoords = fix_coords({shipPos->x, shipPos->y},
                                 {m_camera.getX() + m_realCamX, m_camera.getY()});
    vec2 levelBorder = {level.max_left, level.max_right};
    bool nearLeft = shipCoords.x <= (levelBorder.x + m_screenWidth);
    bool nearRight = shipCoords.x >= (levelBorder.y - m_screenWidth);

    std::shared_ptr<Entity> levelEnt;
    std::shared_ptr<LevelComponent> levelComp;
    if (nearLeft || nearRight) {
        levelEnt = m_entities["level"];
        levelComp = levelEnt->getComponent<LevelComponent>();
    }

    if (nearLeft) {
        level.extendToLeft(m_camera);
        levelComp->points = level.points;
        levelComp->platforms = level.platforms;
        levelComp->stars = level.stars;
    } else if (nearRight) {
        level.extendToRight(m_camera);
        levelComp->points = level.points;
        levelComp->platforms = level.platforms;
        levelComp->stars = level.stars;
    }

    //bool nearUp = shipCoords.y <= (level.stars.back().y + m_screenWidth);
    //if (nearUp) {
    //}
}

void World::update_text()
{
    if constexpr (debug) {
        auto fpsEntity = m_entities["fpsText"];
        auto textFps = fpsEntity->getComponent<TextComponent>();
        textFps->texture->setText((format("FPS: %+3d") % m_fps.get_fps()).str());
    }

    auto velxEntity = m_entities["velxText"];
    auto velyEntity = m_entities["velyText"];
    auto altEntity = m_entities["altitude"];
    auto fuelEntity = m_entities["fuel"];
    auto timeEntity = m_entities["time"];

    auto textVelX = velxEntity->getComponent<TextComponent>();
    auto textVelY = velyEntity->getComponent<TextComponent>();
    auto textAlt = altEntity->getComponent<TextComponent>();
    auto textFuel = fuelEntity->getComponent<TextComponent>();
    auto textTime = timeEntity->getComponent<TextComponent>();

    if (getGameState() == GameStates::NORMAL
        || getGameState() == GameStates::WIN) {
        const auto shipEntity = m_entities["ship"];
        const auto points = m_entities["level"]->getComponent<LevelComponent>()->points;

        const auto shipVel = shipEntity->getComponent<VelocityComponent>();
        const auto shipPos = shipEntity->getComponent<PositionComponent>();
        const auto fuel = shipEntity->getComponent<LifeTimeComponent>();

        textVelX->texture->setText((format("Horizontal speed: %5d") %
                                    floor(shipVel->x * 60.f)).str());
        textVelY->texture->setText((format("Vertical speed: %5d") %
                                    floor(-shipVel->y * 60.f)).str());
        textAlt->texture->setText((format("Altitude: %5d") %
                                   floor(altitude(points, shipPos->x, shipPos->y)
                                         - SHIP_HEIGHT)).str());
        textFuel->texture->setText((format("Fuel: %5d") % fuel->time).str());
        textTime->texture->setText((format("Time: %5f")
                                    % (m_timer.getTicks() / 1000.f)).str());

        if ((getGameState() == GameStates::WIN
             || getGameState() == GameStates::FAIL) && m_timer.isStarted())
            m_timer.pause();

        if (getGameState() == GameStates::WIN
            && m_entities.count("winText") == 0) {
            Entity& winText = createEntity("winText");
            winText.addComponents<TextComponent, PositionComponent>();
            winText.activate();

            auto winTextTexture = winText.getComponent<TextComponent>();
            TTF_Font* font = open_font(msgFont, 14);
            GLfloat score = fuel->time * 1.f / m_timer.getTicks() * 1000.f;
            std::string winMsg = (format("You landed\nScore is %.3f\n"
                                         "To play again press Enter")
                                  % score).str();
            winTextTexture->texture =
                    make_shared<TextTexture>(winMsg, font, fontColor);

            auto winTextPos = winText.getComponent<PositionComponent>();
            winTextPos->x = m_screenWidth / 2.f
                            - winTextTexture->texture->getWidth() / 2.f;
            winTextPos->y = m_screenHeight / 2.f
                            - winTextTexture->texture->getHeight() / 2.f;
            winTextPos->scallable = false;
        }
    } else if (getGameState() == GameStates::FAIL
               && m_entities.count("failText") == 0) { // Fail case
        textVelX->texture->setText((format("Horizontal speed: %3d") % 0).str());
        textVelY->texture->setText((format("Vertical speed: %3d") % 0).str());

        Entity& failText = createEntity("failText");
        failText.addComponents<TextComponent, PositionComponent>();
        failText.activate();

        auto failTextTexture = failText.getComponent<TextComponent>();
        TTF_Font* font = open_font(msgFont, 14);
        failTextTexture->texture =
                make_shared<TextTexture>("You fail\n"
                                         "To play again press Enter", font,
                                         fontColor);

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

    if (getGameState() == GameStates::WIN
        && getPrevGameState() != GameStates::WIN) {
        m_systems[type_id<MovementSystem>()]->stop();
        m_entities["ship"]->removeComponent<KeyboardComponent>();
    }

    if (getGameState() == GameStates::NORMAL
        || getGameState() == GameStates::WIN)
        update_ship();

    update_text();
    update_level();

    filter_entities();
    for (auto &system: m_systems)
        system.second->update(delta);
}

void World::init()
{
    //TODO: fix this
    level.height_min = utils::getScreenHeight<GLfloat>()
                       - utils::getScreenHeight<GLfloat>() / 2.f;
    level.height_max = utils::getScreenHeight<GLfloat>();

    if (!m_wasInit) {
        m_screenWidth = utils::getScreenWidth<GLuint>();
        m_screenHeight = utils::getScreenHeight<GLuint>();
        m_frameWidth = m_screenWidth;
        m_frameHeight = m_screenHeight;

        createSystem<RendererSystem>();
        createSystem<MovementSystem>();
        createSystem<KeyboardSystem>();
        createSystem<AnimationSystem>();
        createSystem<CollisionSystem>();
        createSystem<PhysicsSystem>();
        createSystem<ParticleRenderSystem>();

        // Order of initialization is matter
        init_level();
        init_ship();
        init_sprites();
        init_text();
        init_sound(); // TODO: fix this
        init_sound();

        m_nonStatic["level"] = m_entities["level"];
        m_nonStatic["ship"] = m_entities["ship"];

        auto shipPos = m_entities["ship"]->getComponent<PositionComponent>();
        m_camera.lookAt(shipPos->x - m_screenWidth / 2.f,
                        shipPos->y - m_screenHeight / 2.f);
        move_from_camera(); // TODO: bug here

        m_wasInit = true;
    } else {
        m_entities.erase("winText");
        m_entities.erase("failText");
        m_entities.erase("ship");
        m_entities.erase("ship particle");
        m_systems[type_id<MovementSystem>()]->start();

        rescale_world();
        init_ship();
        m_nonStatic["ship"] = m_entities["ship"];
        m_realCamX += m_camera.getX();
        m_camera.lookAt(0.f, 0.f); // reset camera
        move_from_camera();

        auto shipPos = m_entities["ship"]->getComponent<PositionComponent>();

        m_camera.lookAt(shipPos->x - m_screenWidth / 2.f,
                        shipPos->y - m_screenHeight / 2.f);
        move_from_camera();
    }

    if (m_timer.isStarted() || m_timer.isPaused())
        m_timer.stop();

    m_timer.start();
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
    const std::string earthPath = "lunar_lander_bw.png";
    earthSprite->sprite = make_shared<Sprite>(
            utils::getResourcePath(earthPath));
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
        TTF_Font *font = open_font(msgFont, 14);
        fspTexture->texture = make_shared<TextTexture>("FPS: 000", font,
                                                       fontColor);

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
    TTF_Font *font = open_font(msgFont, 14);
    velxTexture->texture =
            make_shared<TextTexture>("Horizontal speed: -000.000", font,
                                     fontColor);

    auto velxPos = velxText.getComponent<PositionComponent>();
    velxPos->x = m_screenWidth - m_screenWidth / 4.2f;
    velxPos->y = m_screenHeight / 10.f;
    velxPos->scallable = false;

    // Velocity y entity
    Entity &velyText = createEntity("velyText");
    velyText.addComponents<TextComponent, PositionComponent>();
    velyText.activate();

    auto velyTexture = velyText.getComponent<TextComponent>();
    font = open_font(msgFont, 14);
    velyTexture->texture =
            make_shared<TextTexture>("Vertical speed: -000.000", font,
                                     fontColor);

    auto velyPos = velyText.getComponent<PositionComponent>();
    velyPos->x = m_screenWidth - m_screenWidth / 4.2f;
    velyPos->y = m_screenHeight / 8.f;
    velyPos->scallable = false;

    Entity &altitude = createEntity("altitude");
    altitude.addComponents<TextComponent, PositionComponent>();
    altitude.activate();

    auto altTexture = altitude.getComponent<TextComponent>();
    font = open_font(msgFont, 14);
    altTexture->texture = make_shared<TextTexture>("Altitude: -000.000", font,
                                                   fontColor);

    auto altPos = altitude.getComponent<PositionComponent>();
    altPos->x = m_screenWidth - m_screenWidth / 4.2f;
    altPos->y = m_screenHeight / 7.f;
    altPos->scallable = false;

    Entity &fuel = createEntity("fuel");
    fuel.addComponents<PositionComponent, TextComponent>();
    fuel.activate();

    auto fuelTexture = fuel.getComponent<TextComponent>();
    font = open_font(msgFont, 14);
    fuelTexture->texture = make_shared<TextTexture>("Fuel: -000.000", font,
                                                    fontColor);

    auto fuelPos = fuel.getComponent<PositionComponent>();
    fuelPos->x = m_screenWidth - m_screenWidth / 4.2f;
    fuelPos->y = m_screenHeight / 6.f;
    fuelPos->scallable = false;

    Entity& time = createEntity("time");
    time.addComponents<PositionComponent, TextComponent>();
    time.activate();

    auto timeTexture = time.getComponent<TextComponent>();
    font = open_font(msgFont, 14);
    timeTexture->texture = make_shared<TextTexture>("Time: -000.000", font,
                                                    fontColor);

    auto timePos = time.getComponent<PositionComponent>();
    timePos->x = m_screenWidth / 15.f;
    timePos->y = m_screenHeight / 15.f;
    timePos->scallable = false;
}

void World::init_level()
{
    Entity& levelEnt = createEntity("level");
    levelEnt.addComponents<LevelComponent, CollisionComponent>();
    levelEnt.activate();

    auto levelComponent = levelEnt.getComponent<LevelComponent>();
    level.extendToRight(m_camera);
    level.extendToLeft(m_camera);
    levelComponent->points = level.points;
    levelComponent->stars = level.stars;
    levelComponent->platforms = level.platforms;
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
    GLfloat alt = utils::physics::altitude(
            m_entities["level"]->getComponent<LevelComponent>()->points,
            shipPos->x, ship_init_alt);
    shipPos->y = alt; // TODO: fix strange height
    shipPos->angle = pi<GLfloat>() / 2.f;

    auto fuel = ship.getComponent<LifeTimeComponent>();
    fuel->time = 1500;

    auto shipVel = ship.getComponent<VelocityComponent>();
    shipVel->x = 2.f;
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
            shipAnim->cur_state = 0;
            m_audio.haltChannel(engine_channel, true);
        }

        if (state[SDL_SCANCODE_LEFT])
            shipVel->angle -= rot_step;

        if (state[SDL_SCANCODE_RIGHT])
            shipVel->angle += rot_step;
    };
}

void World::move_from_camera()
{
    for (const auto& [_, en]: m_nonStatic) {
        auto pos = en->getComponent<PositionComponent>();
        if (pos) {
            pos->x -= m_camera.deltaX();
            pos->y -= m_camera.deltaY();
        } else {
            auto levelComp = en->getComponent<LevelComponent>();
            for (auto& point : levelComp->points) {
                point.x -= m_camera.deltaX();
                point.y -= m_camera.deltaY();
            }
            for (auto& star : levelComp->stars) {
                star.x -= m_camera.deltaX();
                star.y -= m_camera.deltaY();
            }
            for (auto& platform: levelComp->platforms) {
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

TTF_Font* World::open_font(const std::string& fontName, size_t fontSize)
{
    TTF_Font* font = TTF_OpenFont(getResourcePath(fontName).c_str(), fontSize);
    if (!font)
        throw SdlException((format("Unable to load font %s\nError: %s\n")
                            % fontName % TTF_GetError()).str(),
                           program_log_file_name(), Category::INTERNAL_ERROR);

    return font;
}


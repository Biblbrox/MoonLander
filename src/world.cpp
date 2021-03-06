#include <memory>
#include <glm/gtc/constants.hpp>
#include <boost/format.hpp>
#include <glm/trigonometric.hpp>

#include "base.hpp"
#include "world.hpp"
#include "components/positioncomponent.hpp"
#include "components/spritecomponent.hpp"
#include "components/velocitycomponent.hpp"
#include "components/levelcomponent.hpp"
#include "systems/renderersystem.hpp"
#include "systems/movementsystem.hpp"
#include "components/textcomponent.hpp"
#include "systems/keyboardsystem.hpp"
#include "systems/animationsystem.hpp"
#include "systems/collisionsystem.hpp"
#include "systems/physicssystem.hpp"
#include "particle/particleengine.hpp"
#include "systems/particlerendersystem.hpp"
#include "utils/random.hpp"
#include "components/lifetimecomponent.hpp"
#include "game.hpp"
#include "exceptions/sdlexception.hpp"

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
using ecs::types::type_id;

const int SHIP_WIDTH = 20;
const int SHIP_HEIGHT = 21;

const int engine_channel = 0;
const int crash_sound_channel = 1;

const int engine_idx = 0;
const int crash_idx = 1;

const char* const msgFont = "kenvector_future2.ttf";
const SDL_Color fontColor = {0xFF, 0xFF, 0xFF, 0xFF};

const GLfloat ship_init_alt = 500;

const size_t ship_id =          0;
const size_t level_id =         1;
const size_t ship_particle_id = 2;
const size_t fps_id =           3;
const size_t velx_id = 		4;
const size_t vely_id = 		5;
const size_t alt_id = 		6;
const size_t fuel_id =		7;
const size_t time_id =		8;
const size_t earth_id =         9;
const size_t fail_id =          10;
const size_t win_id =           11;

void World::rescale_world()
{
    m_frameWidth = m_scaled ? m_screenWidth : (m_screenWidth / m_scaleFactor);
    m_frameHeight = m_scaled ? m_screenHeight : (m_screenHeight / m_scaleFactor);

    auto levelEnt = m_entities[level_id];
    levelEnt->getComponent<LevelComponent>()->scale_factor = m_scaled ?
                                                          1.f : m_scaleFactor;

    if (m_entities.count(ship_id) != 0) {
        auto ship = m_entities[ship_id];
        size_t idx = type_id<RendererSystem>;
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
    update_movables();
}

void World::update_ship()
{
    using utils::physics::altitude;
    using utils::Position;

    auto ship = m_entities[ship_id];
    auto shipPos = ship->getComponent<PositionComponent>();
    auto shipVel = ship->getComponent<VelocityComponent>();

    auto renderSystem = std::dynamic_pointer_cast<RendererSystem>(
            m_systems[type_id<RendererSystem>]
    );
    auto levelComp = m_entities[level_id]->getComponent<LevelComponent>();
    GLfloat shipAlt = altitude(levelComp->points, shipPos->x, shipPos->y);
    const GLfloat alt_threshold = 100.f; // Threshold when world will be scaled
    if ((shipAlt < alt_threshold && !m_scaled) // Need to increase scale
        || (shipAlt >= alt_threshold && m_scaled)) { // Need to decrease scale
        rescale_world();
    }

    if ((shipPos->x >= m_frameWidth - m_frameWidth / 4.f)
        || (shipPos->x < m_frameWidth / 4.f)) { // Horizontal edges
        m_camera.translate(shipVel->x, 0.f);
        update_movables();
    }

    if ((shipPos->y >= m_frameHeight - m_frameHeight / 4.f)
               || (shipPos->y < m_frameHeight / 4.f)) { // Vertical edges
        m_camera.translate(0.f, shipVel->y);
        update_movables();
    }

    auto colShip = ship->getComponent<CollisionComponent>();
    if (colShip->has_collision) {
        const vector<vec2>& platforms = levelComp->platforms;
        bool landed = false;
        assert(platforms.size() % 2 == 0);
        GLfloat angle = shipPos->angle -
                        glm::two_pi<GLfloat>()
                        * std::floor(shipPos->angle / glm::two_pi<GLfloat>());
        GLfloat crit_angle = glm::pi<GLfloat>() / 6.f;
        if (((angle >= -crit_angle && angle <= crit_angle)
             || (angle >= glm::two_pi<GLfloat>() - crit_angle))
            && std::abs(shipVel->y * 60.f) <= 20) {
            const GLfloat pad = 2;
            utils::Rect shipSize = ship->getComponent<SpriteComponent>()->sprite->getCurrentClip();
            for (size_t i = 0; i < platforms.size() - 1; i += 2) {
                GLfloat left_bound = platforms[i].x;
                GLfloat right_bound = platforms[i + 1].x;
                if (shipPos->x >= left_bound - pad
                    && shipPos->x <= right_bound + pad
                    && shipPos->x + shipSize.w <= right_bound + pad) {
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

            auto particle = ParticleEngine::generateFromTexture<4 * 4>(
                    utils::getResourcePath("lunar_lander_bw.png"),
                    generate_clips<4, 4>(shipClip), coords, vel, 10000.f);

            m_entities.emplace(ship_particle_id, particle);

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

    auto ship = m_entities[ship_id];
    const auto shipPos = ship->getComponent<PositionComponent>();

    vec2 shipCoords = fix_coords({shipPos->x, shipPos->y},
                                 {m_camera.getX() + m_realCamX, m_camera.getY()});
    vec2 levelBorder = {level.max_left, level.max_right};
    bool nearLeft = shipCoords.x <= (levelBorder.x + m_screenWidth);
    bool nearRight = shipCoords.x >= (levelBorder.y - m_screenWidth);

    std::shared_ptr<Entity> levelEnt;
    std::shared_ptr<LevelComponent> levelComp;
    if (nearLeft || nearRight) {
        levelEnt = m_entities[level_id];
        levelComp = levelEnt->getComponent<LevelComponent>();
    }

    if (nearLeft) { // TODO: there is a bug: coordinate y jump when level extends
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
        auto fpsEntity = m_entities[fps_id];
        auto textFps = fpsEntity->getComponent<TextComponent>();
        textFps->texture->setText((format("FPS: %+3d") % m_fps.get_fps()).str());
    }

    auto velxEntity = m_entities[velx_id];
    auto velyEntity = m_entities[vely_id];
    auto altEntity = m_entities[alt_id];
    auto fuelEntity = m_entities[fuel_id];
    auto timeEntity = m_entities[time_id];

    auto textVelX = velxEntity->getComponent<TextComponent>();
    auto textVelY = velyEntity->getComponent<TextComponent>();
    auto textAlt = altEntity->getComponent<TextComponent>();
    auto textFuel = fuelEntity->getComponent<TextComponent>();
    auto textTime = timeEntity->getComponent<TextComponent>();

    if (getGameState() == GameStates::NORMAL
        || getGameState() == GameStates::WIN) {
        const auto shipEntity = m_entities[ship_id];
        const auto points = m_entities[level_id]->getComponent<LevelComponent>()->points;

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
            && m_entities.count(win_id) == 0) {
            auto winText = createEntity(win_id);
            winText->addComponents<TextComponent, PositionComponent>();
            winText->activate();

            auto winTextTexture = winText->getComponent<TextComponent>();
            TTF_Font* font = open_font(msgFont, 14);
            GLfloat score = fuel->time * 1.f / m_timer.getTicks() * 1000.f;
            std::string winMsg = (format("You landed\nScore is %.3f\n"
                                         "To play again press Enter")
                                  % score).str();
            winTextTexture->texture =
                    make_shared<TextTexture>(winMsg, font, fontColor);

            auto winTextPos = winText->getComponent<PositionComponent>();
            winTextPos->x = m_screenWidth / 2.f
                            - winTextTexture->texture->getWidth() / 2.f;
            winTextPos->y = m_screenHeight / 2.f
                            - winTextTexture->texture->getHeight() / 2.f;
            winTextPos->scallable = false;
        }
    } else if (getGameState() == GameStates::FAIL
               && m_entities.count(fail_id) == 0) { // Fail case
        textVelX->texture->setText((format("Horizontal speed: %3d") % 0).str());
        textVelY->texture->setText((format("Vertical speed: %3d") % 0).str());

        auto failText = createEntity(fail_id);
        failText->addComponents<TextComponent, PositionComponent>();
        failText->activate();

        auto failTextTexture = failText->getComponent<TextComponent>();
        TTF_Font* font = open_font(msgFont, 14);
        failTextTexture->texture =
                make_shared<TextTexture>("You fail\n"
                                         "To play again press Enter", font,
                                         fontColor);

        auto failTextPos = failText->getComponent<PositionComponent>();
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
        m_systems[type_id<MovementSystem>]->stop();
        m_entities[ship_id]->removeComponent<KeyboardComponent>();
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
        init_sound();

        m_nonStatic[level_id] = m_entities[level_id];
        m_nonStatic[ship_id] = m_entities[ship_id];

        auto shipPos = m_entities[ship_id]->getComponent<PositionComponent>();
        m_camera.lookAt(shipPos->x - m_screenWidth / 2.f,
                        shipPos->y - m_screenHeight / 2.f);
        update_movables();

        m_wasInit = true;
    } else {
        m_entities.erase(win_id);
        m_entities.erase(fail_id);
        m_entities.erase(ship_id);
        m_entities.erase(ship_particle_id);
        m_systems[type_id<MovementSystem>]->start();

        rescale_world();
        init_ship();
        
        m_nonStatic[ship_id] = m_entities[ship_id];
        m_realCamX += m_camera.getX();
        m_camera.lookAt(0.f, 0.f); // reset camera
        update_movables();

        auto shipPos = m_entities[ship_id]->getComponent<PositionComponent>();

        m_camera.lookAt(shipPos->x - m_screenWidth / 2.f,
                        shipPos->y - m_screenHeight / 2.f);
        update_movables();
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

    m_audio.setMusic(utils::getResourcePath("mus.wav"));
    m_audio.playMusic();
}

void World::init_sprites()
{
    using namespace utils::physics;

    auto earth = createEntity(earth_id);
    earth->addComponents<PositionComponent, SpriteComponent>();
    earth->activate();

    auto earthSprite = earth->getComponent<SpriteComponent>();
    const std::string earthPath = "lunar_lander_bw.png";
    earthSprite->sprite = make_shared<Sprite>(
            utils::getResourcePath(earthPath));
    earthSprite->sprite->addClipSprite({200, 77, 40, 33});
    earthSprite->sprite->generateDataBuffer();

    auto earthPos = earth->getComponent<PositionComponent>();
    earthPos->x = m_screenWidth / 10.f;
    earthPos->y = m_screenHeight / 10.f;
    earthPos->scallable = false;
}

void World::init_text()
{
    if constexpr (debug) {
        // Fps entity
        auto fpsText = createEntity(fps_id);
        fpsText->addComponents<TextComponent, PositionComponent>();
        fpsText->activate();

        auto fspTexture = fpsText->getComponent<TextComponent>();
        TTF_Font *font = open_font(msgFont, 14);
        fspTexture->texture = make_shared<TextTexture>("FPS: 000", font,
                                                       fontColor);

        auto fpsPos = fpsText->getComponent<PositionComponent>();
        fpsPos->x = m_screenWidth - m_screenWidth / 4.2f;
        fpsPos->y = m_screenHeight / 15.f;
        fpsPos->scallable = false;
    }

    // Velocity x entity
    auto velxText = createEntity(velx_id);
    velxText->addComponents<TextComponent, PositionComponent>();
    velxText->activate();

    auto velxTexture = velxText->getComponent<TextComponent>();
    TTF_Font *font = open_font(msgFont, 14);
    velxTexture->texture =
            make_shared<TextTexture>("Horizontal speed: -000.000", font,
                                     fontColor);

    auto velxPos = velxText->getComponent<PositionComponent>();
    velxPos->x = m_screenWidth - m_screenWidth / 4.2f;
    velxPos->y = m_screenHeight / 10.f;
    velxPos->scallable = false;

    // Velocity y entity
    auto velyText = createEntity(vely_id);
    velyText->addComponents<TextComponent, PositionComponent>();
    velyText->activate();

    auto velyTexture = velyText->getComponent<TextComponent>();
    font = open_font(msgFont, 14);
    velyTexture->texture =
            make_shared<TextTexture>("Vertical speed: -000.000", font,
                                     fontColor);

    auto velyPos = velyText->getComponent<PositionComponent>();
    velyPos->x = m_screenWidth - m_screenWidth / 4.2f;
    velyPos->y = m_screenHeight / 8.f;
    velyPos->scallable = false;

    auto altitude = createEntity(alt_id);
    altitude->addComponents<TextComponent, PositionComponent>();
    altitude->activate();

    auto altTexture = altitude->getComponent<TextComponent>();
    font = open_font(msgFont, 14);
    altTexture->texture = make_shared<TextTexture>("Altitude: -000.000", font,
                                                   fontColor);

    auto altPos = altitude->getComponent<PositionComponent>();
    altPos->x = m_screenWidth - m_screenWidth / 4.2f;
    altPos->y = m_screenHeight / 7.f;
    altPos->scallable = false;

    auto fuel = createEntity(fuel_id);
    fuel->addComponents<PositionComponent, TextComponent>();
    fuel->activate();

    auto fuelTexture = fuel->getComponent<TextComponent>();
    font = open_font(msgFont, 14);
    fuelTexture->texture = make_shared<TextTexture>("Fuel: -000.000", font,
                                                    fontColor);

    auto fuelPos = fuel->getComponent<PositionComponent>();
    fuelPos->x = m_screenWidth - m_screenWidth / 4.2f;
    fuelPos->y = m_screenHeight / 6.f;
    fuelPos->scallable = false;

    auto time = createEntity(time_id);
    time->addComponents<PositionComponent, TextComponent>();
    time->activate();

    auto timeTexture = time->getComponent<TextComponent>();
    font = open_font(msgFont, 14);
    timeTexture->texture = make_shared<TextTexture>("Time: -000.000", font,
                                                    fontColor);

    auto timePos = time->getComponent<PositionComponent>();
    timePos->x = m_screenWidth / 15.f;
    timePos->y = m_screenHeight / 15.f;
    timePos->scallable = false;
}

void World::init_level()
{
    auto levelEnt = createEntity(level_id);
    levelEnt->addComponents<LevelComponent, CollisionComponent>();
    levelEnt->activate();

    auto levelComponent = levelEnt->getComponent<LevelComponent>();
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
    auto ship = createEntity(ship_id);
    ship->addComponents<PositionComponent, SpriteComponent, VelocityComponent,
            KeyboardComponent, AnimationComponent, CollisionComponent,
            /*fuel*/ LifeTimeComponent>();
    ship->activate();

    auto shipSprite = ship->getComponent<SpriteComponent>();
    shipSprite->sprite = make_shared<Sprite>(
            utils::getResourcePath("lunar_lander_bw.png"));
    shipSprite->sprite->addClipSprite({0, 32, SHIP_WIDTH, SHIP_HEIGHT});
    shipSprite->sprite->addClipSprite({20, 32, SHIP_WIDTH, SHIP_HEIGHT});
    shipSprite->sprite->addClipSprite({40, 32, SHIP_WIDTH, SHIP_HEIGHT});
    shipSprite->sprite->generateDataBuffer();

    auto shipPos = ship->getComponent<PositionComponent>();
    shipPos->x = m_screenWidth / 2.f;
    GLfloat alt = utils::physics::altitude(
            m_entities[level_id]->getComponent<LevelComponent>()->points,
            shipPos->x, ship_init_alt);
    shipPos->y = alt;
    shipPos->angle = pi<GLfloat>() / 2.f;

    auto fuel = ship->getComponent<LifeTimeComponent>();
    fuel->time = 1500;

    auto shipVel = ship->getComponent<VelocityComponent>();
    shipVel->x = 2.f;
    auto shipAnim = ship->getComponent<AnimationComponent>();

    auto keyboardComponent = ship->getComponent<KeyboardComponent>();
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

void World::update_movables()
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


#ifndef MOONLANDER_WORLD_HPP
#define MOONLANDER_WORLD_HPP

#include <unordered_map>
#include <memory>
#include <string>
#include <SDL_ttf.h>

#include "utils/fps.hpp"
#include "level.hpp"
#include "utils/timer.hpp"
#include "utils/utils.hpp"
#include "ecs/basesystem.hpp"
#include "render/camera.hpp"
#include "utils/audio.hpp"
#include "ecs/ecsmanager.hpp"

using ecs::Entity;

class World: public ecs::EcsManager
{
public:
    World() : m_scaled(false), m_wasInit(false), m_realCamX(0.f){};
    ~World() = default;

    void init() override;
    void update(size_t delta) override;

private:
    std::unordered_map<size_t, std::shared_ptr<Entity>> m_nonStatic;
    Camera m_camera;
    GLuint m_screenHeight;
    GLuint m_screenWidth;
    GLfloat m_frameHeight;
    GLfloat m_frameWidth;

    // Need to save camera pos after ship dead
    // Because camera will be reset.
    GLfloat m_realCamX;

    utils::Timer m_timer;

    utils::Fps m_fps;

    /**
     * Update position of components
     */
    void update_movables();

    void update_ship();
    void update_text();
    void update_level();
    void rescale_world();
    void init_sound();
    void init_sprites();
    void init_text();
    void init_level();
    void init_ship();
    TTF_Font* open_font(const std::string& font, size_t fontSize);

    /**
     * Remove all entities that not alive
     */
    void filter_entities();

    bool m_scaled;
    const GLfloat m_scaleFactor = 1.5f;
    utils::audio::Audio m_audio;
    Level level;

    bool m_wasInit;
};

#endif //MOONLANDER_WORLD_HPP

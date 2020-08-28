#ifndef MOONLANDER_RENDERERSYSTEM_HPP
#define MOONLANDER_RENDERERSYSTEM_HPP

#include <GL/glew.h>
#include <moonlanderprogram.hpp>
#include <camera.hpp>
#include <components/textcomponent.hpp>
#include "ces/system.hpp"
#include "components/displaycomponent.hpp"
#include "components/positioncomponent.hpp"

/**
 * System that can handle level surface
 */
class RendererSystem : public System<system_holder<DisplayComponent, PositionComponent, TextComponent>>
{
public:
    explicit RendererSystem();

    void update(size_t delta) override;
private:
    MoonLanderProgram program;
    Camera camera;

    std::vector<Entity> movables;

    GLfloat scale_factor = 1.f;
    GLuint screen_width;
    GLuint screen_height;
};

#endif //MOONLANDER_RENDERERSYSTEM_HPP

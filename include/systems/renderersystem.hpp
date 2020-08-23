#ifndef MOONLANDER_RENDERERSYSTEM_HPP
#define MOONLANDER_RENDERERSYSTEM_HPP

#include <GL/glew.h>
#include <moonlanderprogram.hpp>
#include <camera.hpp>
#include <components/textcomponent.h>
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
    void setScale(GLfloat scale);
private:
    MoonLanderProgram program;
    Camera camera;

    std::vector<Entity> movables;

    GLfloat scale_factor = 1.f;
};

#endif //MOONLANDER_RENDERERSYSTEM_HPP

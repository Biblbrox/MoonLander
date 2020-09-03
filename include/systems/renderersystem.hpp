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
class RendererSystem : public System<DisplayComponent, PositionComponent, TextComponent>
{
public:
    explicit RendererSystem();

    void update(size_t delta) override;
private:
    Camera camera;

    GLuint screen_width;
    GLuint screen_height;

    void drawSprites();
    void drawLevel();
    void drawText();
};

#endif //MOONLANDER_RENDERERSYSTEM_HPP

#ifndef MOONLANDER_RENDERERSYSTEM_HPP
#define MOONLANDER_RENDERERSYSTEM_HPP

#include <GL/glew.h>

#include "moonlanderprogram.hpp"
#include "render/camera.hpp"
#include "components/textcomponent.hpp"
#include "ecs/system.hpp"
#include "components/positioncomponent.hpp"

/**
 * System that can handle level surface
 */
class RendererSystem : public ecs::System<PositionComponent, TextComponent>
{
public:
    explicit RendererSystem();

    void update_state(size_t delta) override;
private:
    void drawSprites();
    void drawLevel();
    void drawText();
    void renderSceneQuad();

    GLuint m_blurBuffer;
    GLuint m_directBuffer;
    GLuint m_blurTexture;
    GLuint m_directTexture;
    GLuint m_pingpongFBO[2];
    GLuint m_pingpongTextures[2];
};

#endif //MOONLANDER_RENDERERSYSTEM_HPP

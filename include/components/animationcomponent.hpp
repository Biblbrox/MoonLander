#ifndef MOONLANDER_ANIMATIONCOMPONENT_HPP
#define MOONLANDER_ANIMATIONCOMPONENT_HPP

#include <GL/glew.h>

#include "ecs/component.hpp"

struct AnimationComponent : ecs::Component
{
    GLuint cur_state = 0;
};

#endif //MOONLANDER_ANIMATIONCOMPONENT_HPP

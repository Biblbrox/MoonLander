#ifndef MOONLANDER_VELOCITYCOMPONENT_HPP
#define MOONLANDER_VELOCITYCOMPONENT_HPP

#include <GL/glew.h>

#include "ecs/component.hpp"

struct VelocityComponent : ecs::Component
{
    GLfloat x = 0.f;
    GLfloat y = 0.f;
    GLfloat angle = 0.f;
};

#endif //MOONLANDER_VELOCITYCOMPONENT_HPP

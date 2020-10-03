#ifndef MOONLANDER_LIFETIMECOMPONENT_H
#define MOONLANDER_LIFETIMECOMPONENT_H

#include <ces/component.hpp>
#include <GL/glew.h>

struct LifeTimeComponent : Component
{
    GLfloat time;
};

#endif //MOONLANDER_LIFETIMECOMPONENT_H
#ifndef MOONLANDER_LIFETIMECOMPONENT_PPH
#define MOONLANDER_LIFETIMECOMPONENT_PPH

#include <GL/glew.h>

#include "ecs/component.hpp"

struct LifeTimeComponent : ecs::Component
{
    GLfloat time;
};

#endif //MOONLANDER_LIFETIMECOMPONENT_PPH
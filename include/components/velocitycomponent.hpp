#ifndef MOONLANDER_VELOCITYCOMPONENT_HPP
#define MOONLANDER_VELOCITYCOMPONENT_HPP

#include <GL/glew.h>
#include "ces/component.hpp"

struct VelocityComponent : Component {
    GLfloat x = 0.f;
    GLfloat y = 0.f;
    GLfloat vel_angle = 0.f;
    bool is_moving = false;
};

#endif //MOONLANDER_VELOCITYCOMPONENT_HPP
#ifndef MOONLANDER_VELOCITYCOMPONENT_HPP
#define MOONLANDER_VELOCITYCOMPONENT_HPP

#include <GL/gl.h>
#include "ces/component.hpp"

struct VelocityComponent : Component {
    GLfloat x;
    GLfloat y;
    GLfloat vel_angle;
    bool is_moving;
};

#endif //MOONLANDER_VELOCITYCOMPONENT_HPP

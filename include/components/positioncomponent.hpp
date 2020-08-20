#ifndef MOONLANDER_POSITIONCOMPONENT_HPP
#define MOONLANDER_POSITIONCOMPONENT_HPP

#include <GL/gl.h>
#include "ces/component.hpp"

struct PositionComponent : Component {
    GLfloat x;
    GLfloat y;
    GLfloat angle;
};

#endif //MOONLANDER_POSITIONCOMPONENT_HPP

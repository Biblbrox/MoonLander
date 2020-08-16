#ifndef MOONLANDER_POSITIONCOMPONENT_HPP
#define MOONLANDER_POSITIONCOMPONENT_HPP

#include <GL/gl.h>
#include "ces/component.hpp"

class PositionComponent : public Component {
    PositionComponent() = default;

private:
    GLfloat x;
    GLfloat y;
};

#endif //MOONLANDER_POSITIONCOMPONENT_HPP

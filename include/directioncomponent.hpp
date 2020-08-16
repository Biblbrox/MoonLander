#ifndef MOONLANDER_DIRECTIONCOMPONENT_HPP
#define MOONLANDER_DIRECTIONCOMPONENT_HPP

#include <GL/gl.h>
#include "ces/component.hpp"

class DirectionComponent : public Component {
    DirectionComponent() = default;

private:
    GLfloat x;
    GLfloat y;
};

#endif //MOONLANDER_DIRECTIONCOMPONENT_HPP

#ifndef MOONLANDER_ANIMATIONCOMPONENT_HPP
#define MOONLANDER_ANIMATIONCOMPONENT_HPP

#include <ces/component.hpp>
#include <GL/glew.h>

struct AnimationComponent : Component {
    GLuint cur_state = 0;
};

#endif //MOONLANDER_ANIMATIONCOMPONENT_HPP

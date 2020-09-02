#ifndef MOONLANDER_PARTICLECOMPONENT_H
#define MOONLANDER_PARTICLECOMPONENT_H

#include <ces/component.hpp>
#include <GL/glew.h>

struct ParticleComponent : Component {
    bool is_alive = false;
    GLfloat life_time;
};

#endif //MOONLANDER_PARTICLECOMPONENT_H

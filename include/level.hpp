#ifndef MOONLANDER_LEVEL_HPP
#define MOONLANDER_LEVEL_HPP

#include <vector>
#include <glm/vec2.hpp>
#include <GL/glew.h>
#include "camera.hpp"

using glm::vec2;

class Level
{
public:
    explicit Level();
    ~Level();
    void extendToRight(const Camera& camera);
    void extendToLeft(const Camera& camera);

    // Need to be set after sdl initialized
    GLfloat height_min;
    GLfloat height_max;

    GLfloat max_left;
    GLfloat max_right;

    std::vector<vec2> platforms;
    std::vector<vec2> points;
    std::vector<vec2> stars;
};


#endif //MOONLANDER_LEVEL_HPP

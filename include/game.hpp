#ifndef MOONLANDER_GAME_HPP
#define MOONLANDER_GAME_HPP

#include <GL/gl.h>
#include "window.hpp"

class Game {
public:
    Game() = delete;

    static void initOnceSDL2();
    static void initGL(GLfloat width, GLfloat height);
};

#endif //MOONLANDER_GAME_HPP

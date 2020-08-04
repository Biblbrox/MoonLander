#ifndef MOONLANDER_GAME_H
#define MOONLANDER_GAME_H

#include <GL/gl.h>
#include "window.hpp"

class Game {
public:
    Game() = delete;

    static void initOnceSDL2();
    static void validateSDL2Errors();
    static void initGL(GLfloat width, GLfloat height);
};

#endif //MOONLANDER_GAME_H

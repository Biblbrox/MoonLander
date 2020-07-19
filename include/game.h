#ifndef MOONLANDER_GAME_H
#define MOONLANDER_GAME_H

#include "window.hpp"

class Game {
public:
    Game() = delete;

    static void initOnceSDL2();
    static void validateSDL2Errors();
    static void initGL(int width, int height);
};

#endif //MOONLANDER_GAME_H

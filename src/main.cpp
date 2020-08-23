#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include <texttexture.hpp>
#include <sprite.hpp>
#include <game.hpp>
#include <moonlanderprogram.hpp>
#include "models/levelgenerator.hpp"
#include <glm/gtc/constants.hpp>
#include <iomanip>
#include "../include/ces/world.hpp"

const int SHIP_WIDTH = 20;
const int SHIP_HEIGHT = 21;

using namespace Utils;

int main(int argc, char *args[])
{
    Game game;
    game.initOnceSDL2();
    game.initGL();
    game.initGame();

    GLuint screen_width = Utils::getScreenWidth<GLuint>();
    GLuint screen_height = Utils::getScreenHeight<GLuint>();

    SDL_Event e;
    while (game.isRunnable()) {
        glViewport(0.f, 0.f, screen_width, screen_height);
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
                game.setRunnable(false);

        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game.update(1 / 60.f);
        game.flush();
    }

    game.quit();

    return 0;
}

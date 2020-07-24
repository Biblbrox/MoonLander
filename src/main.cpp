#include <GL/glew.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <sstream>
#include <GL/gl.h>

#include <texttexture.h>
#include <sprite.h>
#include <models/ship.hpp>
#include <game.h>
#include <keyboardhandler.h>
#include "models/level.hpp"
#include "../include/window.hpp"
#include "../include/texture.hpp"
#include "../include/utils.hpp"
#include "../include/timer.h"

#define WINDOW_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN

const int SHIP_WIDTH = 20;
const int SHIP_HEIGHT = 21;

const GLfloat gravity_force = 0.5f;
const GLfloat weight = 40.f;
const GLfloat engine_force = 1.f;
const GLfloat rot_step = 0.05f;

using namespace Utils;

int main(int argc, char *args[]) {
    //Init SDL2
    Game::initOnceSDL2();
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    int screen_width = dm.w;
    int screen_height = dm.h;

    Window window(GAME_NAME.c_str(), SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, WINDOW_FLAGS);

    // Init OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window.getWindow());
    if (glContext == NULL) {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
    }

    // Init OpenGL
    Game::initGL(screen_width, screen_height);

    Level level;

    TTF_Font *fpsFont = TTF_OpenFont(
            getResourcePath("kenvector_future2.ttf").c_str(), 14);
    Timer fpsTimer;
    std::stringstream fpsText;
    TextTexture fpsTexture("FPS: ", {0xFF, 0xFF, 0xFF, 0xFF}, fpsFont);
    Uint32 countFrames = 0;
    fpsTimer.start();

    /**
     * Need to synchronize moving per second
     */

    Sprite shipSprite(getResourcePath("lunar_lander_bw.png"));
    shipSprite.addClipSprite({.x = 0,  .y = 57, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    shipSprite.addClipSprite({.x = 20, .y = 57, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    shipSprite.addClipSprite({.x = 40, .y = 57, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    shipSprite.generateDataBuffer();

    Ship ship(&shipSprite, 0.1f, 0);
    ship.setCoords({.x = 10, .y = 10});

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
                quit = true;

        const Uint8* state = SDL_GetKeyboardState(nullptr);

        float avgFPS = countFrames / (fpsTimer.getTicks() / 1000.f);
        if (avgFPS > 200000)
            avgFPS = 0;

        fpsText.str("");
        fpsText << "FPS: " << avgFPS;
        fpsTexture.setText(fpsText.str());

        glClear(GL_COLOR_BUFFER_BIT);

        // Update physics
        if (state[SDL_SCANCODE_UP]) {
            if (!ship.enginesOn())
                ship.turnEngines();

            ship.addVelY(-engine_force / weight *
                         std::sin(radians(ship.getAngle() + 90)));
            ship.addVelX(-engine_force / weight *
                         std::cos(radians(ship.getAngle() + 90)));

            ship.setSprite((SDL_GetTicks() / 100) % 2 + 1);
        } else {
            ship.setSprite(0);
            if (ship.enginesOn())
                ship.turnEngines();
        }

        if (state[SDL_SCANCODE_LEFT])
            ship.addVelRot(-rot_step);

        if (state[SDL_SCANCODE_RIGHT])
            ship.addVelRot(rot_step);

        ship.addCoords({.x = ship.getVelX(), .y = ship.getVelY()});
        ship.rotate(ship.getVelRot());
        ship.addVelY(gravity_force / weight);
        if (level.hasCollision(ship.getCoords()))
            ship.setCoords({.x = ship.getX(), .y = ship.getY() - 1});
        // End updating physics

        // Update graphic scene
        level.render();
        ship.render();
        fpsTexture.render(screen_width - 200.f, screen_height / 20.f, nullptr);

        glFlush();
        SDL_GL_SwapWindow(window.getWindow());
        // End updating graphic scene
        ++countFrames;
    }

    SDL_GL_DeleteContext(glContext);
    TTF_CloseFont(fpsFont);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

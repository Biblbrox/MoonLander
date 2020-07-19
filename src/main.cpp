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
#include "models/level.hpp"
#include "../include/window.hpp"
#include "../include/texture.hpp"
#include "../include/utils.hpp"
#include "../include/timer.h"

#define WINDOW_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 550;

const int SHIP_WIDTH = 20;
const int SHIP_HEIGHT = 20;

const GLfloat gravity_force = 0.5f;
const GLfloat weight = 40.f;
const GLfloat engine_force = 5.f;
const GLfloat rot_step = 0.5f;

using namespace Utils;

int main(int argc, char *args[]) {
    //Init SDL2
    Game::initOnceSDL2();

    Window window(GAME_NAME.c_str(), SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_FLAGS);

    // Init OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window.getWindow());
    if (glContext == NULL) {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
    }

    // Init OpenGL
    Game::initGL(SCREEN_WIDTH, SCREEN_HEIGHT);

    Level level;

    TTF_Font *fpsFont = TTF_OpenFont(getResourcePath("kenvector_future2.ttf").c_str(), 14);
    Timer fpsTimer;
    std::stringstream fpsText;
    TextTexture fpsTexture("FPS: ", {0xFF, 0xFF, 0xFF, 0xFF}, fpsFont);
    Uint32 countFrames = 0;
    fpsTimer.start();

    /**
     * Need to synchronize moving per second
     */
    Timer moveTimer;

    Sprite shipSprite(getResourcePath("lunar_lander.png"));
    shipSprite.addClipSprite({.x = 0, .y = 57, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    shipSprite.addClipSprite({.x = 20, .y = 57, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    shipSprite.addClipSprite({.x = 40, .y = 57, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    shipSprite.generateDataBuffer();

    Ship ship(&shipSprite, 0, 0);
    ship.setCoords({.x = 10, .y = 10});

    SDL_Event e;
    bool quit = false;
    moveTimer.start();
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if (!ship.enginesOn())
                            ship.turnEngines();

                        ship.addVelY(-engine_force / weight * std::sin(radians(ship.getAngle() + 90)));
                        ship.addVelX(-engine_force / weight * std::cos(radians(ship.getAngle() + 90)));
                        break;
                    case SDLK_DOWN:
                        break;
                    case SDLK_LEFT:
                        ship.addVelRot(-rot_step);
                        break;
                    case SDLK_RIGHT:
                        ship.addVelRot(rot_step);
                        break;
                    default:
                        break;
                }
            } else if (e.type == SDL_KEYUP) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if (ship.enginesOn())
                            ship.turnEngines();
                        break;
                    default:
                        break;
                }
            }
        }

        float avgFPS = countFrames / (fpsTimer.getTicks() / 1000.f);
        if (avgFPS > 200000)
            avgFPS = 0;

        fpsText.str("");
        fpsText << "FPS: " << avgFPS;
        fpsTexture.setText(fpsText.str());

        glClear(GL_COLOR_BUFFER_BIT);

        // Update physics
        ship.addCoords({.x = ship.getVelX(), .y = ship.getVelY()});
        ship.rotate(ship.getVelRot());
        ship.addVelY(gravity_force / weight);
        // End updating physics

        // Update graphic scene
        level.render();
        ship.render();
        fpsTexture.render(SCREEN_WIDTH - SCREEN_WIDTH / 5.f, SCREEN_HEIGHT / 20.f, nullptr);

        glFlush();
        SDL_GL_SwapWindow(window.getWindow());
        // End updating graphic scene

        ++countFrames;
    }

    SDL_GL_DeleteContext(glContext);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <sstream>
#include <GL/glew.h>
#include <GL/gl.h>

#include <texttexture.h>
#include <imagetexture.h>
#include <models/ship.hpp>
#include <game.h>
#include "models/level.hpp"
#include "../include/renderer.hpp"
#include "../include/window.hpp"
#include "../include/texture.hpp"
#include "../include/utils.hpp"
#include "../include/timer.h"

//#define VSYNC

#define WINDOW_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
#ifdef VSYNC
#define RENDER_FLAGS SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
#else
#define RENDER_FLAGS SDL_RENDERER_ACCELERATED
#endif

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 550;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

const int SHIP_WIDTH = 20;
const int SHIP_HEIGHT = 20;

using Utils::cleanup;
using Utils::getResourcePath;
using Utils::Timer;


int main(int argc, char* args[]) {
    //Init SDL2
    Game::initOnceSDL2();

    Window window(GAME_NAME.c_str(), SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_FLAGS);

    // Init OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window.getWindow());
    if( glContext == NULL )
    {
        printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
    }

    // Init OpenGL
    Game::initGL(SCREEN_WIDTH, SCREEN_HEIGHT);

    Level level;

   // TTF_Font* fpsFont = TTF_OpenFont(getResourcePath("kenvector_future2.ttf").c_str(), 14);
    Timer fpsTimer;
    std::stringstream fpsText;
    //TextTexture fpsTexture(&renderer, "FPS: ", {0xFF, 0xFF, 0xFF, 0xFF}, fpsFont);
    Uint32 countFrames = 0;
    fpsTimer.start();

#ifndef VSYNC
    Timer capTimer;
#endif

    /**
     * Need to synchronize moving per second
     */
    Timer moveTimer;

    ImageTexture shipTexture(getResourcePath("lunar_lander.png"));
    Rect shipClip{.x = 0, .y = 57, .w = SHIP_WIDTH, .h = SHIP_HEIGHT};
    Ship ship(&shipTexture, &shipClip, 0, 0);
    ship.setCoords({.x = 10, .y = 10});

    SDL_Event e;
    bool quit = false;
    moveTimer.start();
    while (!quit) {
#ifndef VSYNC
        capTimer.start();
#endif
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
            if (moveTimer.getTicks() <= 1000) {
                if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
                    switch (e.key.keysym.sym) {
                        case SDLK_UP:
                            ship.setVelY(ship.getVelY() - 5);
                            break;
                        case SDLK_DOWN:
                            ship.setVelY(ship.getVelY() + 5);
                            break;
                        case SDLK_LEFT:
                            ship.setVelX(ship.getVelX() - 5);
                            ship.setVelRot(ship.getVelRot()  - 5);
                            break;
                        case SDLK_RIGHT:
                            ship.setVelX(ship.getVelX() + 5);
                            ship.setVelRot(ship.getVelRot() + 5);
                            break;
                        default:
                            break;
                    }
                } else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
                    switch (e.key.keysym.sym) {
                        case SDLK_UP:
                            ship.setVelY(ship.getVelY() + 5);
                            break;
                        case SDLK_DOWN:
                            ship.setVelY(ship.getVelY() - 5);
                            break;
                        case SDLK_LEFT:
                            ship.addVelX(5);
                            ship.addVelRot(5);
                            break;
                        case SDLK_RIGHT:
                            ship.addVelX(-5);
                            ship.addVelRot(-5);
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        // Update graphic scene
        float avgFPS = countFrames / (fpsTimer.getTicks() / 1000.f);
        if (avgFPS > 200000)
            avgFPS = 0;

        fpsText.str("");
        fpsText << "FPS: " << avgFPS;
        //fpsTexture.setText(fpsText.str());

        if (moveTimer.getTicks() <= 1000) {
            // Update physics
            ship.addCoords({.x = ship.getVelX(), .y = ship.getVelY()});
            ship.rotate(ship.getVelRot());
            // End updating physics
        }

        // End updating graphic scene

        // Render updated scene

        //fpsTexture.renderTexture(SCREEN_WIDTH  - SCREEN_WIDTH / 5, SCREEN_HEIGHT / 20, nullptr);

        glClear(GL_COLOR_BUFFER_BIT);

        //background.render(0, 0, &background_rect);
        level.render();
        ship.render();

        glFlush();
        SDL_GL_SwapWindow(window.getWindow());
        // End rendering scene
        ++countFrames;

#ifndef VSYNC
        int frameTicks = capTimer.getTicks();
        if (frameTicks < SCREEN_TICKS_PER_FRAME)
            SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
#endif

        if (moveTimer.getTicks() >= 1000) {
            moveTimer.stop();
            moveTimer.start();
        }
    }

    SDL_GL_DeleteContext(glContext);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

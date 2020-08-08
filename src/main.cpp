#include <GL/glew.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <sstream>

#include <texttexture.h>
#include <sprite.h>
#include <game.h>
#include <moonlanderprogram.h>
#include <models/ship.hpp>
#include "models/level.hpp"
#include "../include/timer.h"

#define WINDOW_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN

const int SHIP_WIDTH = 20;
const int SHIP_HEIGHT = 21;

const GLfloat gravity_force = 0.5f;
const GLfloat weight = 40.f;
const GLfloat engine_force = 1.f;
const GLfloat rot_step = 0.005f;

using namespace Utils;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
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
    if (glContext == nullptr) {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
    }

    // Init OpenGL
    Game::initGL(screen_width, screen_height);

    Level level;

    TTF_Font *fpsFont = TTF_OpenFont(
            getResourcePath("kenvector_future2.ttf").c_str(), 14);
    Timer fpsTimer;
    std::stringstream fpsText;
    TextTexture fpsTexture("FPS: 60",
                           {0xFF, 0xFF, 0xFF, 0xFF}, fpsFont);
    Uint32 countFrames = 0;
    fpsTimer.start();

    /**
     * Need to synchronize moving per second
     */

    MoonLanderProgram program;
    if (!program.loadProgram()) {
        printf("Unable to load basic shader!\n");
    }
    program.bind();

    program.setProjection(glm::ortho<GLfloat>(0.0f, screen_width, screen_height, 0.0f, 1.0f, -1.0f));
    program.updateProjection();
    program.setModel(glm::mat4(1.f));
    program.updateModel();
    program.setView(glm::mat4(1.f));
    program.updateView();
    program.setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
    program.updateColor();
    program.setTexture(0);

    Sprite shipSprite(getResourcePath("lunar_lander_bw.png"));
    shipSprite.addClipSprite({.x = 0,  .y = 32, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    shipSprite.addClipSprite({.x = 20, .y = 32, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    shipSprite.addClipSprite({.x = 40, .y = 32, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    shipSprite.generateDataBuffer();

    Ship ship(&shipSprite, 0.f, 0.f);
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
        fpsText << "FPS: " << std::floor(avgFPS);
        fpsTexture.setText(fpsText.str());

        glClearColor(0.f, 0.f, 0.f, 1.0f);
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

        if (state[SDL_SCANCODE_LEFT] && !ship.isLanded())
            ship.addVelRot(-rot_step);

        if (state[SDL_SCANCODE_RIGHT] && !ship.isLanded())
            ship.addVelRot(rot_step);

        if (level.hasCollision({.x = ship.getX(), .y = ship.getY(),
                                      .w = SHIP_WIDTH,  .h = SHIP_HEIGHT}, ship.getAngle())) {
            if (!ship.isLanded())
                ship.setVel({.x = 0, .y = 0});
            if (!ship.isLanded())
                ship.turnLanded();

            if (ship.enginesOn()) {
                ship.addCoords({.x = ship.getVelX(), .y = ship.getVelY()});
            }
        } else {
            if (ship.isLanded())
                ship.turnLanded();
            ship.addCoords({.x = ship.getVelX(), .y = ship.getVelY()});
            ship.rotate(ship.getVelRot());
            ship.addVelY(gravity_force / weight);
        }
        // End updating physics

        // Update graphic scene
       // Camera::lookAt(ship.getX(), ship.getY(), 1.f, 0.f, 0.f, 0.f);
        program.setTextureRendering(false);
        level.render(program);
        program.setTextureRendering(true);
        ship.render(program);
        fpsTexture.render(program, (GLfloat)screen_width - (GLfloat)screen_width / 9.f, (GLfloat)screen_height / 15.f, nullptr);

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
#pragma clang diagnostic pop

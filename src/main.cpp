#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <sstream>
#include <texttexture.h>
#include <imagetexture.h>
#include "models/level.hpp"
#include "../include/renderer.hpp"
#include "../include/window.hpp"
#include "../include/texture.hpp"
#include "../include/utils.hpp"
#include "../include/timer.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

using Utils::cleanup;
using Utils::getResourcePath;
using Utils::Timer;

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init");
        return 1;
    }

    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );

    int IMG_FLAGS = IMG_INIT_PNG;

    if ((IMG_Init(IMG_FLAGS) & IMG_FLAGS) != IMG_FLAGS) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "IMG_Init");
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() == 1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF_Init");
        SDL_Quit();
    }

    Window window(GAME_NAME.c_str(), SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    Renderer renderer(window, -1,
                      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawColor(renderer.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);

    ImageTexture background(renderer, getResourcePath("black_background.png"));

    Level level;

    TTF_Font* fpsFont = TTF_OpenFont(getResourcePath("kenvector_future2.ttf").c_str(), 14);
    Timer fpsTimer;
    std::stringstream fpsText;
    TextTexture fpsTexture(renderer, "FPS: ", {0xFF, 0xFF, 0xFF, 0xFF}, *fpsFont);
    Uint32 countFrames = 0;
    fpsTimer.start();

    SDL_Event e;
    bool quit = false;
    SDL_Rect background_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_1:
                        break;
                    case SDLK_2:
                        break;
                    case SDLK_3:
                        break;
                    case SDLK_4:
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
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
        fpsTexture.load(fpsText.str(), {0xFF, 0xFF, 0xFF, 0xFF}, *fpsFont);

        renderer.renderClear();
        renderer.setDrawColor(0xFF, 0xFF, 0xFF, 0xFF);
        background.renderTexture(0, 0, &background_rect);
        fpsTexture.renderTexture(SCREEN_WIDTH  - SCREEN_WIDTH / 5, SCREEN_HEIGHT / 20, nullptr);
        level.render(renderer);

        renderer.renderPresent();
        SDL_GL_SwapWindow(window.getWindow());

        ++countFrames;
    }

    cleanup(background);
    cleanup(renderer);
    cleanup(window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

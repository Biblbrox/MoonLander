#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "models/level.hpp"
#include "../include/renderer.hpp"
#include "../include/window.hpp"
#include "../include/texture.hpp"
#include "../include/utils.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

using Utils::cleanup;
using Utils::getResourcePath;

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

    Window window(GAME_NAME.c_str(), SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    Renderer renderer(window, -1,
                      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawColor(renderer.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);

    Texture background(renderer, getResourcePath("black_background.png"));

    Level level;

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

        renderer.renderClear();
        renderer.setDrawColor(0xFF, 0xFF, 0xFF, 0xFF);
        background.renderTexture(0, 0, &background_rect);
        level.render(renderer);

        renderer.renderPresent();
        SDL_GL_SwapWindow(window.getWindow());
    }

    cleanup(background);
    cleanup(renderer);
    cleanup(window);

    IMG_Quit();
    SDL_Quit();

    return 0;
}

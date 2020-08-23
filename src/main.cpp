#include <SDL2/SDL.h>
#include <game.hpp>
#include "../include/utils.hpp"

int main(int argc, char *args[])
{
    /**
     * Set my own log function
     */
    SDL_LogSetOutputFunction(&utils::log::log_function, nullptr);

    try {
        Game game;
        game.initOnceSDL2();
        game.initGL();
        game.initGame();

        SDL_Event e;
        while (game.isRunnable()) {
            while (SDL_PollEvent(&e))
                if (e.type == SDL_QUIT)
                    game.setRunnable(false);

            game.update(1 / 60.f);
            game.flush();
        }

        game.quit();
    } catch (std::exception& e) {
        //TODO: write info to main log file
        printf("%s\n", e.what());
        std::terminate();
    }

    return 0;
}

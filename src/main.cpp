#include "../include/utils.hpp"
#include <SDL2/SDL.h>
#include <game.hpp>

int main(int argc, char *args[])
{
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
        utils::log::Logger::write(utils::program_log_file_name(),
                                  utils::log::Category::UNEXPECTED_ERROR, e.what());
        return -1;
    }

    return 0;
}

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
        int32_t tick_interval = 1000 / 60;
        size_t last_update_time = 0;
        int32_t delta_time = 0;
        size_t cur_time = 0;
        while (game.isRunnable()) {
            if (!game.vsync_supported) {
                cur_time = SDL_GetTicks();
                delta_time = cur_time - last_update_time;

                int32_t time_to_sleep = tick_interval - delta_time;
                if (time_to_sleep > 0)
                    SDL_Delay(time_to_sleep);
            }

            while (SDL_PollEvent(&e))
                if (e.type == SDL_QUIT)
                    game.setRunnable(false);

            game.update(delta_time);
            game.flush();

            if (!game.vsync_supported)
                last_update_time = cur_time;
        }

        game.quit();
    } catch (std::exception& e) {
        utils::log::Logger::write(utils::program_log_file_name(),
                                  utils::log::Category::UNEXPECTED_ERROR, e.what());
        return -1;
    }

    return 0;
}

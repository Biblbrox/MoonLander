#include "../include/utils.hpp"
#include <SDL2/SDL.h>
#include <game.hpp>
#include <moonlanderprogram.hpp>

int main(int argc, char *args[])
{
    try {
        auto game = Game::getInstance();
        game->initOnceSDL2();
        game->initGL();
        game->initGame();

        auto screen_width = utils::getScreenWidth<GLuint>();
        auto screen_height = utils::getScreenHeight<GLuint>();

        auto program = MoonLanderProgram::getInstance();
        program->loadProgram();
        program->bind();
        program->setProjection(glm::ortho<GLfloat>(
                0.0f, screen_width, screen_height,
                0.0f, 1.0f, -1.0f));
        program->setModel(glm::mat4(1.f));
        program->setView(glm::mat4(1.f));
        program->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
        program->updateModel();
        program->updateView();
        program->updateProjection();
        program->updateColor();
        program->setTexture(0);

        SDL_Event e;
        int32_t tick_interval = 1000 / 60;
        size_t last_update_time = 0;
        int32_t delta_time = 0;
        size_t cur_time = 0;
        while (game->isRunnable()) {
            glViewport(0.f, 0.f, screen_width, screen_height);
            glClearColor(0.f, 0.f, 0.f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            if (!game->vsync_supported) {
                cur_time = SDL_GetTicks();
                delta_time = cur_time - last_update_time;

                int32_t time_to_sleep = tick_interval - delta_time;
                if (time_to_sleep > 0)
                    SDL_Delay(time_to_sleep);
            }

            while (SDL_PollEvent(&e))
                if (e.type == SDL_QUIT)
                    game->setRunnable(false);

            game->update(delta_time);
            game->flush();

            if (!game->vsync_supported)
                last_update_time = cur_time;
        }

        game->quit();
    } catch (std::exception& e) {
        utils::log::Logger::write(utils::program_log_file_name(),
                                  utils::log::Category::UNEXPECTED_ERROR, e.what());
        return -1;
    }

    return 0;
}

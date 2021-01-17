#include <SDL2/SDL.h>
#include <glm/gtc/matrix_transform.hpp>

#include "game.hpp"
#include "moonlanderprogram.hpp"
#include "utils/logger.hpp"
#include "exceptions/basegameexception.hpp"

#ifndef NDEBUG // use callgrind profiler
#include <valgrind/callgrind.h>
#endif

using utils::log::program_log_file_name;
using utils::log::Category;

int main(int argc, char *args[])
{
    int ret_code = 0;
    try {
        Game game;
        game.initOnceSDL2();
        game.initGL();
        game.initGame();

        auto screen_width = utils::getScreenWidth<GLuint>();
        auto screen_height = utils::getScreenHeight<GLuint>();
        auto program = MoonLanderProgram::getInstance();
        program->loadProgram();
        program->setProjection(glm::ortho<GLfloat>(
                0.0f, screen_width, screen_height, 0.0f, 1.0f, -1.0f));
        program->setModel(glm::mat4(1.f));
        program->setView(glm::mat4(1.f));
        program->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
        program->updateModel();
        program->updateView();
        program->updateProjection();
        program->updateColor();
        program->setTexture(0);

        program->switchToLinesAdj();

        SDL_Event e;
        int32_t tick_interval = 1000 / 60;
        size_t last_update_time = 0;
        int32_t delta_time = 0;
        size_t cur_time = 0;

#ifndef NDEBUG
            CALLGRIND_START_INSTRUMENTATION;
            CALLGRIND_TOGGLE_COLLECT;
#endif
        while (isGameRunnable()) {
            glViewport(0.f, 0.f, screen_width, screen_height);
            glClearColor(0.f, 0.f, 0.f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            if (!game.vsync_supported) {
                cur_time = SDL_GetTicks();
                delta_time = cur_time - last_update_time;

                int32_t time_to_sleep = tick_interval - delta_time;
                if (time_to_sleep > 0)
                    SDL_Delay(time_to_sleep);
            }

            while (SDL_PollEvent(&e))
                if (e.type == SDL_QUIT)
                    setGameRunnable(false);

            game.update(delta_time);
            game.flush();

            if (!game.vsync_supported)
                last_update_time = cur_time;
        }

    } catch (const BaseGameException& e) {
        utils::log::Logger::write(e.fileLog(), e.categoryError(), e.what());
        ret_code = EXIT_FAILURE;
    } catch (const std::exception& e) {
        utils::log::Logger::write(program_log_file_name(),
                                  Category::UNEXPECTED_ERROR, e.what());
        ret_code = EXIT_FAILURE;
    }

    quit();

#ifndef NDEBUG
        CALLGRIND_TOGGLE_COLLECT;
        CALLGRIND_STOP_INSTRUMENTATION;
#endif

    return ret_code;
}

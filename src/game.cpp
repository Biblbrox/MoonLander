#include <GL/glew.h>
#include <game.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glm/geometric.hpp>
#include <memory>
#include <constants.hpp>
#include <functional>

using utils::log::Logger;
using boost::format;

void Game::initOnceSDL2()
{
    static bool didInit = false;

    if (!didInit) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            Logger::write(utils::program_log_file_name(),
                          utils::log::Category::INITIALIZATION_ERROR,
                          "Can't init SDL\n");
            std::abort();
        }

        int IMG_FLAGS = IMG_INIT_PNG;

        if ((IMG_Init(IMG_FLAGS) & IMG_FLAGS) != IMG_FLAGS) {
            Logger::write(utils::program_log_file_name(),
                          utils::log::Category::INITIALIZATION_ERROR,
                          "Can't init SDL_IMG\n");
            SDL_Quit();
            std::abort();
        }

        if (TTF_Init() == -1) {
            Logger::write(utils::program_log_file_name(),
                          utils::log::Category::INITIALIZATION_ERROR,
                          "Can't init SDL_TTF\n");
            SDL_Quit();
            std::abort();
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    }
}


Game::Game() : is_runnable(true), vsync_supported(false)
{
    glContext = nullptr;
}

void Game::update(size_t delta)
{
    world.update(delta);
}

void Game::initGL()
{
    screen_w = utils::getScreenWidth<GLuint>();
    screen_h = utils::getScreenHeight<GLuint>();

    using deleterType = typename std::function<void(SDL_Window*)>;
    auto deleter = [](SDL_Window *win){
        SDL_DestroyWindow(win);
    };
    window = std::unique_ptr<SDL_Window, deleterType>(
            SDL_CreateWindow(GAME_NAME.c_str(), SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h,
                             WINDOW_FLAGS), deleter);

    glContext = SDL_GL_CreateContext(window.get());
    // Init OpenGL context
    if (glContext == nullptr) {
        Logger::write(utils::program_log_file_name(),
                      utils::log::Category::INITIALIZATION_ERROR,
                      (format("OpenGL context could not be created! SDL Error: %s\n")
                       % SDL_GetError()).str());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        std::abort();
    }

    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if (error != GLEW_OK) {
        Logger::write(utils::program_log_file_name(),
                      utils::log::Category::INITIALIZATION_ERROR,
                      (format("Error when initializing GLEW: %s\n")
                       % glewGetErrorString(error)).str());
        quit();
        std::abort();
    }

    if (!GLEW_VERSION_2_1) {
        Logger::write(utils::program_log_file_name(),
                      utils::log::Category::INITIALIZATION_ERROR,
                      (format("Opengl 2.1 not supported\n")).str());
        quit();
        std::abort();
    }

    //Initialize clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);

    // TODO: fix ugly lines
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glLineWidth(1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Use Vsync
    if(SDL_GL_SetSwapInterval( -1 ) < 0)
    {
        Logger::write(utils::program_log_file_name(),
                      utils::log::Category::INITIALIZATION_ERROR,
                      (format("Warning: Unable to enable VSync. "
                              "VSync will not be used! SDL Error: %s\n")
                       % SDL_GetError()).str());
        SDL_GL_SetSwapInterval(0);
        vsync_supported = false;
    } else {
        vsync_supported = true;
    }

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        Logger::write(utils::program_log_file_name(),
                      utils::log::Category::INITIALIZATION_ERROR,
                      (format("Error initializing OpenGL(Final)! %s\n")
                       % gluErrorString(error)).str());
        quit();
        std::abort();
    }
}

void Game::flush()
{
    glFlush();
    SDL_GL_SwapWindow(window.get());
}

void Game::quit()
{
    SDL_GL_DeleteContext(glContext);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::setRunnable(bool runnable)
{
    is_runnable = runnable;
}

bool Game::isRunnable() const
{
    return is_runnable;
}

void Game::initGame()
{
    world.init();
}



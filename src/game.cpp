#include <GL/glew.h>
#include <game.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glm/geometric.hpp>
#include <memory>
#include <types.hpp>

void Game::initOnceSDL2()
{
    static bool didInit = false;

    if (!didInit) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            SDL_Log( "SDL_Init");
            std::abort();
        }

        int IMG_FLAGS = IMG_INIT_PNG;

        if ((IMG_Init(IMG_FLAGS) & IMG_FLAGS) != IMG_FLAGS) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "IMG_Init");
            SDL_Quit();
            std::abort();
        }

        if (TTF_Init() == -1) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF_Init");
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


Game::Game() : is_runnable(true)
{
    glContext = nullptr;
}

void Game::update(size_t delta)
{
    world.update(delta);
}

void Game::initGL()
{
    screen_w = Utils::getScreenWidth<GLuint>();
    screen_h = Utils::getScreenHeight<GLuint>();

    window = std::make_unique<Window>(GAME_NAME.c_str(), SDL_WINDOWPOS_UNDEFINED,
           SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h, WINDOW_FLAGS);

    glContext = SDL_GL_CreateContext(window->getWindow());
    // Init OpenGL context
    if (glContext == nullptr) {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        std::abort();
    }

    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if (error != GLEW_OK) {
        SDL_Log("Error when initializing GLEW: %s\n", glewGetErrorString(error));
        std::abort();
    }

    if (!GLEW_VERSION_2_1) {
        SDL_Log("Opengl 2.1 not supported\n");
        std::abort();
    }

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL(Ortho/Viewport)! %s\n", gluErrorString( error ) );
    }

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL(MatrixMode)! %s\n", gluErrorString( error ) );
    }

    //Initialize clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Use Vsync
    if(SDL_GL_SetSwapInterval( 1 ) < 0)
    {
        SDL_Log( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
    }

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL(Final)! %s\n", gluErrorString( error ) );
    }
}

void Game::flush()
{
    glFlush();
    SDL_GL_SwapWindow(window->getWindow());
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



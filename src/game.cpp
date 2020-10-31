#include <GL/glew.h>
#include <game.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <constants.hpp>

#define FREQUENCY 44100
#define SAMPLE_FORMAT MIX_DEFAULT_FORMAT
#define CHUNK_SIZE 2048
#define NUM_CHANNELS 2

static bool imgInit = false;
static bool mixerInit = false;

static SDL_Window* m_window;
static SDL_GLContext m_glcontext;

static bool isRun = true;
static GameStates state = GameStates::NORMAL;
static GameStates prevState = GameStates::NORMAL;

using utils::log::Logger;
using boost::format;

void quit()
{
    // TODO: fix destructor call
    if (m_glcontext)
        SDL_GL_DeleteContext(m_glcontext);
    if (TTF_WasInit())
        TTF_Quit();
    if (imgInit)
        IMG_Quit();
    if (m_window)
        SDL_DestroyWindow(m_window);
    if (mixerInit)
        Mix_Quit();

    SDL_Quit();
}

void setGameRunnable(bool runnable)
{
    isRun = runnable;
}

bool isGameRunnable()
{
    return isRun;
}

GameStates getGameState()
{
    return state;
}

void setGameState(GameStates st)
{
    prevState = state;
    state = st;
}

GameStates getPrevGameState()
{
    return prevState;
}


void Game::initOnceSDL2()
{
    static bool didInit = false;

    if (!didInit) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
            Logger::write(utils::program_log_file_name(),
                          utils::log::Category::INITIALIZATION_ERROR,
                          "Unable to init SDL\n");
            std::abort();
        }

        if ((IMG_Init(IMG_FLAGS) & IMG_FLAGS) != IMG_FLAGS) {
            Logger::write(utils::program_log_file_name(),
                          utils::log::Category::INITIALIZATION_ERROR,
                          "Unable to init SDL_IMG\n");
            quit();
            std::abort();
        }
        imgInit = true;

        if (Mix_OpenAudio(FREQUENCY, SAMPLE_FORMAT,
                          NUM_CHANNELS, CHUNK_SIZE) < 0) {
            Logger::write(utils::program_log_file_name(),
                          utils::log::Category::INITIALIZATION_ERROR,
                          "Unable to init SDL_Mixer\n");
            quit();
            std::abort();

        }
        mixerInit = true;

        if (TTF_Init() == -1) {
            Logger::write(utils::program_log_file_name(),
                          utils::log::Category::INITIALIZATION_ERROR,
                          "Can't init SDL_TTF\n");
            quit();
            std::abort();
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    }
}


Game::Game() : vsync_supported(false)
{
    m_glcontext = nullptr;
    m_window = nullptr;
}

void Game::update(size_t delta)
{
    if (getGameState() == GameStates::NEED_REPLAY) {
        m_world.init(); // Reinit world
        setGameState(GameStates::NORMAL);
    }

    m_world.update(delta);
}

void Game::initGL()
{
    m_screenWidth = utils::getScreenWidth<GLuint>();
    m_screenHeight = utils::getScreenHeight<GLuint>();

    m_window = SDL_CreateWindow(GAME_NAME.c_str(),
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                m_screenWidth, m_screenHeight,
                                WINDOW_FLAGS);
    if (!m_window) {
        Logger::write(utils::program_log_file_name(),
                      utils::log::Category::INITIALIZATION_ERROR,
                      (format("Unable to create window! "
                              "SDL Error: %s\n") % SDL_GetError()).str());
        quit();
        std::abort();
    }

    m_glcontext = SDL_GL_CreateContext(m_window);
    // Init OpenGL context
    if (!m_glcontext) {
        Logger::write(utils::program_log_file_name(),
                      utils::log::Category::INITIALIZATION_ERROR,
                      (format("OpenGL context could not be created! "
                              "SDL Error: %s\n") % SDL_GetError()).str());
        quit();
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

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glLineWidth(1.0f);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Use Vsync
    if(SDL_GL_SetSwapInterval(-1) < 0) {
        Logger::write(utils::program_log_file_name(),
                      utils::log::Category::INITIALIZATION_ERROR,
                      (format("Warning: Unable to enable VSync. "
                              "VSync will not be used! SDL Error: %s\n")
                       % SDL_GetError()).str());
        // If can't use vsync try set fixed fps
        SDL_GL_SetSwapInterval(0);
        vsync_supported = false;
    } else {
        vsync_supported = true;
    }

    //Check for error
    error = glGetError();
    if(error != GL_NO_ERROR) {
        Logger::write(utils::program_log_file_name(),
                      utils::log::Category::INITIALIZATION_ERROR,
                      (format("Error initializing OpenGL! %s\n")
                       % gluErrorString(error)).str());
        quit();
        throw GLException("Error while initGL!");
    }
}

void Game::flush()
{
    glFlush();
    SDL_GL_SwapWindow(m_window);
}

void Game::initGame()
{
    m_world.init();
}

Game::~Game()
{

}


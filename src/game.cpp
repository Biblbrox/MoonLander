#include <game.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

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

        if (TTF_Init() == 1) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF_Init");
            SDL_Quit();
            std::abort();
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    }
}

void Game::initGL(const int width, const int height)
{
    GLenum error = glewInit();
    if (error != GLEW_OK)
        SDL_Log("Error when initializing GLEW: %s\n", glewGetErrorString(error));

    if (!GLEW_VERSION_2_1)
        SDL_Log("Opengl 2.1 not supported\n");

    glViewport(0.f, 0.f, width,  height);
    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, 1.0, -1.0);

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
    }

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
    }

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    glEnable(GL_TEXTURE_2D);
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_POLYGON_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Use Vsync
    if( SDL_GL_SetSwapInterval( 1 ) < 0 )
    {
        printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
    }

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
    }
}


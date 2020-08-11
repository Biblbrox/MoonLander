#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include <texttexture.hpp>
#include <sprite.hpp>
#include <game.hpp>
#include <moonlanderprogram.hpp>
#include <models/ship.hpp>
#include "models/level.hpp"

#define WINDOW_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN

const int SHIP_WIDTH = 20;
const int SHIP_HEIGHT = 21;

const GLfloat gravity_force = 0.5f;
const GLfloat weight = 40.f;
const GLfloat engine_force = 1.f;
const GLfloat rot_step = 0.004f;

using namespace Utils;

int main(int argc, char *args[]) {
    //Init SDL2
    Game::initOnceSDL2();
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    int screen_width = dm.w;
    int screen_height = dm.h;

    Window window(GAME_NAME.c_str(), SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, WINDOW_FLAGS);

    // Init OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window.getWindow());
    if (glContext == nullptr) {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
    }

    // Init OpenGL
    Game::initGL(screen_width, screen_height);

    Camera camera;

    Level level(&camera);

    TTF_Font* textFont = TTF_OpenFont(
            getResourcePath("kenvector_future2.ttf").c_str(), 14);
    Timer fpsTimer;

    char fpsText[8];
    char velxText[12];
    char velyText[12];

    SDL_Color textColor = {0xFF, 0xFF, 0xFF, 0xFF};
    TextTexture fpsTexture("FPS: 60", textColor, textFont);
    TextTexture velxTexture("Vel x: 0.00", textColor, textFont);
    TextTexture velyTexture("Vel y: 0.00", textColor, textFont);
    Uint32 countFrames = 0;
    fpsTimer.start();

    MoonLanderProgram program;
    if (!program.loadProgram()) {
        printf("Unable to load basic shader!\n");
        std::abort();
    }
    program.bind();

    program.setProjection(glm::ortho<GLfloat>(0.0f, screen_width, screen_height, 0.0f, 1.0f, -1.0f));
    program.setModel(glm::mat4(1.f));
    program.setView(glm::mat4(1.f));
    program.setColor(glm::vec4(0.1f, 1.f, 0.1f, 1.f));
    program.updateModel();
    program.updateView();
    program.updateProjection();
    program.updateColor();
    program.setTexture(0);

    Sprite shipSprite(getResourcePath("lunar_lander_bw.png"));
    shipSprite.addClipSprite({.x = 0,  .y = 32, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    shipSprite.addClipSprite({.x = 20, .y = 32, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    shipSprite.addClipSprite({.x = 40, .y = 32, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    shipSprite.generateDataBuffer();

    Ship ship(&camera, &shipSprite, 0.f, 0.f);
    ship.setCoords({.x = 10, .y = 10});

    program.updateView();

    int frame_width = screen_width;
    int frame_height = screen_height;

    SDL_Event e;
    bool scaled = false;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
                quit = true;

        const Uint8* state = SDL_GetKeyboardState(nullptr);

        GLfloat avgFPS = Utils::getFps(fpsTimer, countFrames);

        sprintf(fpsText, "FPS: %.0f", std::floor(avgFPS));
        sprintf(velxText, "Vel x: %.1f", ship.getVelX());
        sprintf(velyText, "Vel y: %.1f", ship.getVelY());
        fpsTexture.setText(fpsText);
        velxTexture.setText(velxText);
        velyTexture.setText(velyText);

        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update physics
        if (state[SDL_SCANCODE_UP]) {
            if (!ship.enginesOn())
                ship.turnEngines();

            ship.addVelY(-engine_force / weight *
                         std::sin(ship.getAngle() + pi() / 2.f));
            ship.addVelX(-engine_force / weight *
                         std::cos(ship.getAngle() + pi() / 2.f));

            ship.setSprite((SDL_GetTicks() / 100) % 2 + 1);
        } else {
            ship.setSprite(0);
            if (ship.enginesOn())
                ship.turnEngines();
        }

        if (state[SDL_SCANCODE_LEFT] && !ship.isLanded())
            ship.addVelRot(-rot_step);

        if (state[SDL_SCANCODE_RIGHT] && !ship.isLanded())
            ship.addVelRot(rot_step);

        if (level.hasCollision({.x = ship.getX(), .y = ship.getY(),
                                      .w = SHIP_WIDTH,  .h = SHIP_HEIGHT}, ship.getAngle())) {
            if (!ship.isLanded()) {
                ship.setVel({.x = 0, .y = 0});
                ship.setVelRot(0.f);
            }
            if (!ship.isLanded())
                ship.turnLanded();

            if (ship.enginesOn())
                ship.addCoords({.x = ship.getVelX(), .y = ship.getVelY()});
        } else {
            if (ship.isLanded())
                ship.turnLanded();
            ship.addCoords({.x = ship.getVelX(), .y = ship.getVelY()});
            ship.rotate(ship.getVelRot());
            ship.addVelY(gravity_force / weight);
        }
        // End updating physics

        // Update graphic scene
        GLfloat scale_factor = 1.5f;

        if ((ship.getX() - camera.getX()) >= frame_width - frame_width / 4.f) {
            camera.translate(1.f, 0.f);
        }

        if ((ship.getX() - camera.getX()) < frame_width / 4.f) {
            camera.translate(-1.f, 0.f);
        }

        if (ship.getY() > 300 && !scaled) {
            program.setView(glm::scale(program.getView(),
                                       glm::vec3(scale_factor, scale_factor, 1.f)));
            program.updateView();
            camera.lookAt(ship.getX() - 200, ship.getY() - 200);
            scaled = true;
            frame_width /= scale_factor;
        }

        if (ship.getY() <= 300 && scaled) {
            program.setView(glm::scale(program.getView(),
                                       glm::vec3(1 / scale_factor, 1 / scale_factor, 1.f)));
            program.updateView();
            camera.lookAt(0.f, 0.f);
            scaled = false;
            frame_width *= scale_factor;
        }

        program.setTextureRendering(false);
        level.render(program);
        program.setTextureRendering(true);
        ship.render(program);
        fpsTexture.render(program, (GLfloat)screen_width - (GLfloat)screen_width / 9.f,
                          (GLfloat)screen_height / 15.f, nullptr);
        velxTexture.render(program, (GLfloat)screen_width - (GLfloat)screen_width / 9.f,
                          (GLfloat)screen_height / 10.f, nullptr);
        velyTexture.render(program, (GLfloat)screen_width - (GLfloat)screen_width / 9.f,
                          (GLfloat)screen_height / 8.f, nullptr);

        glFlush();
        SDL_GL_SwapWindow(window.getWindow());
        // End updating graphic scene
        ++countFrames;
    }

    SDL_GL_DeleteContext(glContext);
    TTF_CloseFont(textFont);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

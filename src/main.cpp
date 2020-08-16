#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include <texttexture.hpp>
#include <sprite.hpp>
#include <game.hpp>
#include <moonlanderprogram.hpp>
#include <models/ship.hpp>
#include "models/level.hpp"
#include <glm/gtc/constants.hpp>
#include <iomanip>
#include "../include/ces/world.hpp"

const int SHIP_WIDTH = 20;
const int SHIP_HEIGHT = 21;

const GLfloat gravity_force = 0.5f;
const GLfloat weight = 40.f;
const GLfloat engine_force = 1.f;
const GLfloat rot_step = 0.004f;

using namespace Utils;

int main(int argc, char *args[])
{
    Game game;
    game.initOnceSDL2();
    game.initGL();

    GLuint screen_width = Utils::getScreenWidth<GLuint>();
    GLuint screen_height = Utils::getScreenHeight<GLuint>();

    Camera camera;

    Level level(&camera);
    level.extendToLeft();
    level.extendToRight();

    TTF_Font* textFont = TTF_OpenFont(
            getResourcePath("kenvector_future2.ttf").c_str(), 14);
    Timer fpsTimer;

    std::stringstream fpsText;
    std::stringstream velxText;
    std::stringstream velyText;

    SDL_Color textColor = {0x00, 0xFF, 0x00, 0xFF};
    TextTexture fpsTexture("FPS: 60", textColor, textFont);
    TextTexture velxTexture("Horizontal speed x: 00.00", textColor, textFont);
    TextTexture velyTexture("Vertical speed y: 00.00", textColor, textFont);
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
    program.setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
    program.updateModel();
    program.updateView();
    program.updateProjection();
    program.updateColor();
    program.setTexture(0);

    int frame_width = screen_width;
    int frame_height = screen_height;

    glViewport(0.f, 0.f, frame_width, frame_height);

    Sprite shipSprite(getResourcePath("lunar_lander_bw.png"));
    shipSprite.addClipSprite({.x = 0,  .y = 32, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    shipSprite.addClipSprite({.x = 20, .y = 32, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    shipSprite.addClipSprite({.x = 40, .y = 32, .w = SHIP_WIDTH, .h = SHIP_HEIGHT});
    shipSprite.generateDataBuffer();

    Ship ship(&camera, &shipSprite, 0.f, 0.f);
    ship.setCoords({.x = 40, .y = 10});

    program.updateView();

    SDL_Event e;
    bool scaled = false;
    while (game.isRunnable()) {
        glViewport(0.f, 0.f, screen_width, screen_height);
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
                game.setRunnable(false);

        const Uint8* state = SDL_GetKeyboardState(nullptr);

        GLfloat avgFPS = Utils::getFps(fpsTimer, countFrames);

        fpsText.str("");
        velxText.str("");
        velyText.str("");
        fpsText << "FPS: " << std::floor(avgFPS);
        velxText << "Horizontal speed x: " << ship.getVelX() * 60.f << std::setprecision(2);
        velyText << "Vertical speed y: " << -ship.getVelY() * 60.f << std::setprecision(2);
        fpsTexture.setText(fpsText.str());
        velxTexture.setText(velxText.str());
        velyTexture.setText(velyText.str());

        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update physics
        if (state[SDL_SCANCODE_UP]) {
            if (!ship.enginesOn())
                ship.turnEngines();

            ship.addVelY(-engine_force / weight *
                         std::sin(ship.getAngle() + glm::half_pi<GLfloat>()));
            ship.addVelX(-engine_force / weight *
                         std::cos(ship.getAngle() + glm::half_pi<GLfloat>()));

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
            camera.translate(1.f * ship.getVelX(), 0.f);
        }

        if ((ship.getX() - camera.getX()) < frame_width / 4.f) {
            camera.translate(1.f * ship.getVelX(), 0.f);
        }

        if (ship.getY() > 300 && !scaled) {
            program.setView(glm::scale(program.getView(),
                                       glm::vec3(scale_factor, scale_factor, 1.f)));
            program.updateView();
            camera.lookAt(ship.getX() - 200, ship.getY() - 200);
            scaled = true;
            frame_width /= scale_factor;
            frame_height /= scale_factor;
            fpsTexture.setScale(1 / scale_factor);
            velxTexture.setScale(1 / scale_factor);
            velyTexture.setScale(1 / scale_factor);
        }

        if (ship.getY() <= 300 && scaled) {
            program.setView(glm::scale(program.getView(),
                                       glm::vec3(1 / scale_factor, 1 / scale_factor, 1.f)));
            program.updateView();
            camera.lookAt(0.f, 0.f);
            scaled = false;
            frame_width *= scale_factor;
            frame_height *= scale_factor;
            fpsTexture.setScale(1.f);
            velxTexture.setScale(1.f);
            velyTexture.setScale(1.f);
        }

        program.setTextureRendering(false);
        level.render(program);
        program.setTextureRendering(true);
        ship.render(program);
        fpsTexture.render(program, frame_width - frame_width / 4.5f,
                          screen_height / 15.f, nullptr);
        velxTexture.render(program, frame_width - frame_width / 4.5f,
                           screen_height / 10.f, nullptr);
        velyTexture.render(program, frame_width - frame_width / 4.5f,
                           screen_height / 8.f, nullptr);


        game.flush();
        // End updating graphic scene
        ++countFrames;
    }

    TTF_CloseFont(textFont);
    game.quit();

    return 0;
}

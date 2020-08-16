#ifndef MOONLANDER_GAME_HPP
#define MOONLANDER_GAME_HPP

#include <GL/gl.h>
#include <SDL_events.h>
#include <models/level.hpp>
#include <sstream>
#include <models/ship.hpp>
#include <memory>
#include "window.hpp"
#include "camera.hpp"
#include "texttexture.hpp"

#define WINDOW_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN

class Game {
public:
    Game();

    void initOnceSDL2();
    void initGL();
    void flush();
    void quit();

    void update();
    void render();
    void setRunnable(bool runnable);

    bool isRunnable() const;
private:
    GLuint screen_w;
    GLuint screen_h;

    SDL_GLContext glContext;

    bool is_runnable;
    std::unique_ptr<Window> window;
};

#endif //MOONLANDER_GAME_HPP

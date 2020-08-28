#ifndef MOONLANDER_GAME_HPP
#define MOONLANDER_GAME_HPP

#include <GL/glew.h>
#include <SDL_events.h>
#include <ces/world.hpp>
#include <models/levelgenerator.hpp>
#include <sstream>
#include <memory>
#include <functional>
#include "camera.hpp"
#include "texttexture.hpp"

#define WINDOW_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN

class Game {
public:
    Game();

    void initOnceSDL2();
    void initGL();
    void initGame();

    void flush();
    void quit();

    void update(size_t delta);
    void setRunnable(bool runnable);

    bool isRunnable() const;
private:
    GLuint screen_w;
    GLuint screen_h;

    SDL_GLContext glContext;

    bool is_runnable;
    std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>> window;
    World world;
};

#endif //MOONLANDER_GAME_HPP

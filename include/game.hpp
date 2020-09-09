#ifndef MOONLANDER_GAME_HPP
#define MOONLANDER_GAME_HPP

#include <GL/glew.h>
#include <ces/world.hpp>
#include <memory>
#include <functional>

#define WINDOW_FLAGS (SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN)
#define IMG_FLAGS IMG_INIT_PNG

class Game {
private:
    static std::shared_ptr<Game> instance;
public:
    Game();
    static std::shared_ptr<Game> getInstance()
    {
        if (!instance) {
            instance = std::make_shared<Game>();
        }

        return instance;
    }

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

    bool is_runnable;

    SDL_GLContext glContext;
public:
    bool vsync_supported;
private:
    std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>> window;
    World world;
};

#endif //MOONLANDER_GAME_HPP

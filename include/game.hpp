#ifndef MOONLANDER_GAME_HPP
#define MOONLANDER_GAME_HPP

#include <GL/glew.h>
#include <ces/world.hpp>
#include <memory>

#define WINDOW_FLAGS (SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN)
#define IMG_FLAGS IMG_INIT_PNG

enum class GameStates {
    NORMAL,
    WIN,
    FAIL,
    NEED_REPLAY
};

class Game
{
private:
    static std::shared_ptr<Game> instance;
public:
    Game();
    ~Game();
    static std::shared_ptr<Game> getInstance()
    {
        if (!instance)
            instance = std::make_shared<Game>();

        return instance;
    }

    void initOnceSDL2();
    void initGL();
    void initGame();

    void flush();
    void quit();

    void update(size_t delta);
    void setRunnable(bool runnable);
    void setState(GameStates state);
    GameStates getState() const;
    GameStates getPrevState() const;

    bool isRunnable() const;
private:
    GLuint m_screenWidth;
    GLuint m_screenHeight;

    bool m_isRunnable;

    SDL_GLContext m_glcontext;
public:
    bool vsync_supported;
private:
    SDL_Window* m_window;
    World m_world;
    GameStates m_state;
    GameStates m_prevState;
};

#endif //MOONLANDER_GAME_HPP

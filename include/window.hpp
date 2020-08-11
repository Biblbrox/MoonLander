#ifndef MOONLANDER_WINDOW_HPP
#define MOONLANDER_WINDOW_HPP

#include <SDL_types.h>
#include <SDL_video.h>

class Window {
public:
    Window(const char *title, int x, int y, int w,
           int h, Uint32 flags);

    ~Window();

    SDL_Window* getWindow();

    void free();

private:
    SDL_Window* window;
};

#endif //MOONLANDER_WINDOW_HPP

#include <window.hpp>

Window::Window(const char *title, int x, int y, int w, int h, Uint32 flags)
{
    this->window = SDL_CreateWindow(title, x, y, w, h, flags);
}

Window::~Window()
{
    if (!this->window)
        SDL_DestroyWindow(this->window);
}

void Window::free()
{
    if (!this->window)
        SDL_DestroyWindow(this->window);

    this->window = nullptr;
}

SDL_Window *Window::getWindow()
{
    return this->window;
}


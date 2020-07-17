#include "../include/renderer.hpp"

Renderer::Renderer(Window& window, int index, Uint32 flags)
{
    this->renderer = SDL_CreateRenderer(window.getWindow(), index, flags);
}

void Renderer::free()
{
    if (this->renderer)
        SDL_DestroyRenderer(this->renderer);
}

Renderer::~Renderer()
{
    if (this->renderer)
        SDL_DestroyRenderer(this->renderer);

    this->renderer = nullptr;
}

SDL_Renderer *Renderer::getRenderer()
{
    return this->renderer;
}

void Renderer::setDrawColor(SDL_Color color)
{
    this->draw_color = color;
    SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);
}

void Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    this->draw_color = {r, g, b, a};
    SDL_SetRenderDrawColor(this->renderer, r, g,  b, a);
}

void Renderer::renderClear()
{
    SDL_RenderClear(this->renderer);
}

void Renderer::renderPresent()
{
    SDL_RenderPresent(this->renderer);
}

SDL_Color Renderer::getDrawColor() const
{
    return draw_color;
}


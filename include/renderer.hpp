#ifndef MOONLANDER_RENDERER_HPP
#define MOONLANDER_RENDERER_HPP

#include <SDL_video.h>
#include <SDL_render.h>
#include "freelable.hpp"
#include "window.hpp"

class Renderer: public Freelable {
public:
    Renderer(Window& window,
                    int index, Uint32 flags);
    ~Renderer();

    SDL_Renderer* getRenderer();

    void free() override;

    void setDrawColor(SDL_Color color);
    void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    SDL_Color getDrawColor() const;

    void renderPresent();
    void renderClear();

private:
    SDL_Renderer* renderer;
    SDL_Color draw_color;
};

#endif //MOONLANDER_RENDERER_HPP

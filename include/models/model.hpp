#ifndef MOONLANDER_MODEL_HPP
#define MOONLANDER_MODEL_HPP

#include <texture.hpp>
#include "renderer.hpp"

class Model {
public:
    explicit Model(Texture* tex = nullptr, SDL_Rect* clip = nullptr) : texture(tex), clip(clip) {};
    virtual void render(Renderer& renderer) = 0;

protected:
    Texture* texture;
    SDL_Rect* clip;
};

#endif //MOONLANDER_MODEL_HPP

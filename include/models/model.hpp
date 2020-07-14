#ifndef MOONLANDER_MODEL_HPP
#define MOONLANDER_MODEL_HPP

#include <texture.hpp>
#include "renderer.hpp"

class Model {
public:
    Model(Texture* tex = nullptr) : texture(tex) {};
    virtual void render(Renderer& renderer) = 0;

protected:
    Texture* texture;
};

#endif //MOONLANDER_MODEL_HPP

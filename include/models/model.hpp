#ifndef MOONLANDER_MODEL_HPP
#define MOONLANDER_MODEL_HPP

#include <texture.hpp>
#include "renderer.hpp"

using Utils::Rect;

class Model {
public:
    explicit Model(Texture* tex = nullptr, Rect* clip = nullptr) : texture(tex), clip(clip) {};
    virtual void render() = 0;

protected:
    Texture* texture;
    Rect* clip;
};

#endif //MOONLANDER_MODEL_HPP

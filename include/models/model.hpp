#ifndef MOONLANDER_MODEL_HPP
#define MOONLANDER_MODEL_HPP

#include <sprite.h>

using Utils::Rect;

class Model {
public:
    explicit Model(Sprite* tex = nullptr, Rect* clip = nullptr) : texture(tex), clip(clip) {};
    virtual void render() = 0;

protected:
    Sprite* texture;
    Rect* clip;
};

#endif //MOONLANDER_MODEL_HPP

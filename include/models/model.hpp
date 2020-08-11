#ifndef MOONLANDER_MODEL_HPP
#define MOONLANDER_MODEL_HPP

#include <sprite.hpp>
#include <camera.hpp>

using Utils::Rect;

class Model {
public:
    explicit Model(Camera* camera, Sprite* tex = nullptr, Rect* clip = nullptr) : texture(tex), clip(clip) {};
    virtual void render(MoonLanderProgram& program) = 0;

protected:
    Sprite* texture;
    Rect* clip;
};

#endif //MOONLANDER_MODEL_HPP

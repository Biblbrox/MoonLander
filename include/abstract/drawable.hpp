#ifndef MOONLANDER_DRAWABLE_HPP
#define MOONLANDER_DRAWABLE_HPP

#include <sprite.hpp>
#include <camera.hpp>

using Utils::Rect;

template <class TexType>
class Drawable {
public:
    explicit Drawable(Camera* camera, TexType* tex = nullptr, Rect* clip = nullptr) : texture(tex), clip(clip) {};
    virtual void render(MoonLanderProgram& program) = 0;

protected:
    TexType* texture;
    Rect* clip;
};

#endif //MOONLANDER_DRAWABLE_HPP

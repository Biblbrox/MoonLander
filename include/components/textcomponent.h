#ifndef MOONLANDER_TEXTCOMPONENT_H
#define MOONLANDER_TEXTCOMPONENT_H

#include <memory>
#include <texttexture.hpp>
#include "../include/ces/component.hpp"

struct TextComponent : Component {
    std::shared_ptr<TextTexture> texture;
};

#endif //MOONLANDER_TEXTCOMPONENT_H

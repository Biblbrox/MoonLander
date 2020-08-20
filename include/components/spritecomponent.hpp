#ifndef MOONLANDER_SPRITECOMPONENT_HPP
#define MOONLANDER_SPRITECOMPONENT_HPP

#include "../include/sprite.hpp"
#include "../include/ces/component.hpp"

struct SpriteComponent : Component {
    std::shared_ptr<Sprite> sprite;
};

#endif //MOONLANDER_SPRITECOMPONENT_HPP

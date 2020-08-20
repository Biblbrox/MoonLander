#ifndef MOONLANDER_ANIMATIONSYSTEM_HPP
#define MOONLANDER_ANIMATIONSYSTEM_HPP

#include "../../include/ces/system.hpp"
#include "../components/spritecomponent.hpp"
#include "../components/animationcomponent.hpp"

class AnimationSystem : public System<system_holder<SpriteComponent, AnimationComponent>> {
    void update(size_t delta) override;
};

#endif //MOONLANDER_ANIMATIONSYSTEM_HPP

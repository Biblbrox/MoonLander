#ifndef MOONLANDER_KEYBOARDCOMPONENT_HPP
#define MOONLANDER_KEYBOARDCOMPONENT_HPP

#include <ces/component.hpp>
#include <functional>
#include <SDL_quit.h>

struct KeyboardComponent : Component
{
    std::function<void(const Uint8*)> event_handler;
};

#endif //MOONLANDER_KEYBOARDCOMPONENT_HPP

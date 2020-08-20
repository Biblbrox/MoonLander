#ifndef MOONLANDER_KEYBOARDCOMPONENT_H
#define MOONLANDER_KEYBOARDCOMPONENT_H

#include <ces/component.hpp>
#include <functional>
#include <SDL_quit.h>

struct KeyboardComponent : Component {
    std::function<void(const Uint8*)> event_handler;
};

#endif //MOONLANDER_KEYBOARDCOMPONENT_H

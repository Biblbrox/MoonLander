#ifndef MOONLANDER_KEYBOARDHANDLER_HPP
#define MOONLANDER_KEYBOARDHANDLER_HPP

#include <SDL_events.h>
#include <map>

class KeyboardHandler {
public:
    KeyboardHandler() = default;

    void handleEvent(SDL_Event& event);
    bool isKeyDown(SDL_Keycode code);
    bool isKeyUp(SDL_Keycode code);
    void clear();

private:
    std::map<SDL_Keycode, Uint32> keys;
};

#endif //MOONLANDER_KEYBOARDHANDLER_HPP

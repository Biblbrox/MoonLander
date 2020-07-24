#include "../include/keyboardhandler.h"

void KeyboardHandler::handleEvent(SDL_Event &event)
{
    keys[event.key.keysym.sym] = event.type;
}

bool KeyboardHandler::isKeyUp(SDL_Keycode code)
{
    auto it = keys.find(code);
    return (it != keys.end() ? (it->second == SDL_KEYUP) : false);
}

bool KeyboardHandler::isKeyDown(SDL_Keycode code)
{
    auto it = keys.find(code);
    return (it != keys.end() ? (it->second == SDL_KEYDOWN) : false);
}

void KeyboardHandler::clear()
{
    keys.clear();
}



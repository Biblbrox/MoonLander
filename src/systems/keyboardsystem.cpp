#include <systems/keyboardsystem.hpp>

void KeyboardSystem::update(size_t delta)
{
    auto handles = getEntities();
    for (auto& [key, en] : handles) {
        en->getComponent<KeyboardComponent>()->event_handler(
                SDL_GetKeyboardState(nullptr));
    }
}
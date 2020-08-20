#include <systems/keyboardsystem.h>

void KeyboardSystem::update(size_t delta)
{
    std::vector<Entity> handles = getEntities();
    for (auto& en : handles) {
        en.getComponent<KeyboardComponent>()->event_handler(SDL_GetKeyboardState(nullptr));
    }
}
#include <systems/keyboardsystem.hpp>
#include <game.hpp>

void KeyboardSystem::update(size_t delta)
{
    auto handles = getEntities();
    for (auto& [key, en] : handles) {
        en->getComponent<KeyboardComponent>()->event_handler(
                SDL_GetKeyboardState(nullptr));
    }

    const Uint8* state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_ESCAPE]) {
        auto game = Game::getInstance();
        game->setRunnable(false);
    }
}
#include <systems/keyboardsystem.hpp>
#include <game.hpp>

void KeyboardSystem::update_state(size_t delta)
{
    auto handles = getEntities();
    for (auto& [key, en] : handles)
        en->getComponent<KeyboardComponent>()->event_handler(
                SDL_GetKeyboardState(nullptr));

    const Uint8* state = SDL_GetKeyboardState(nullptr);
    auto game = Game::getInstance();

    if (state[SDL_SCANCODE_RETURN]
        && (game->getState() == GameStates::FAIL
            || game->getState() == GameStates::WIN))
        game->setState(GameStates::NEED_REPLAY);

    if (state[SDL_SCANCODE_ESCAPE])
        game->setRunnable(false);
}
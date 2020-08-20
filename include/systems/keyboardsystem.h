#ifndef MOONLANDER_KEYBOARDSYSTEM_H
#define MOONLANDER_KEYBOARDSYSTEM_H

#include <components/keyboardcomponent.h>
#include "../ces/system.hpp"

class KeyboardSystem : public System<system_holder<KeyboardComponent>>
{
    void update(size_t delta) override;
};

#endif //MOONLANDER_KEYBOARDSYSTEM_H

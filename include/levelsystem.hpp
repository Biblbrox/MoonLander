#ifndef MOONLANDER_LEVELSYSTEM_HPP
#define MOONLANDER_LEVELSYSTEM_HPP

#include "ces/system.hpp"
#include "surfacecomponent.hpp"

/**
 * System that can handle level surface
 */
class LevelSystem : public System<system_holder<SurfaceComponent>>
{
public:
    void update();
    void render();
};

#endif //MOONLANDER_LEVELSYSTEM_HPP

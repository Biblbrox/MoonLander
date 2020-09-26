#ifndef MOONLANDER_BASESYSTEM_HPP
#define MOONLANDER_BASESYSTEM_HPP

#include <memory>

class World;

/**
 * System base class
 */
class BaseSystem
{
public:
    virtual void setWorld(std::shared_ptr<World> world) final
    {
        m_world = world;
    }

    virtual void update(size_t delta) = 0;

protected:
    std::shared_ptr<World> m_world;
};

#endif //MOONLANDER_BASESYSTEM_HPP

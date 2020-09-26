#ifndef MOONLANDER_COMPONENT_HPP
#define MOONLANDER_COMPONENT_HPP

#include <memory>

class World;

/**
 * Base class for component
 */
class Component
{
public:
    Component() = default;
    virtual ~Component() = default;

    virtual void setWorld(std::shared_ptr<World> world) final;

private:
    std::shared_ptr<World> m_world;
};

#endif //MOONLANDER_COMPONENT_HPP

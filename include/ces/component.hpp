#ifndef MOONLANDER_COMPONENT_HPP
#define MOONLANDER_COMPONENT_HPP

class World;

/**
 * Base class for component
 */
class Component
{
public:
    Component() : m_world(nullptr) {};
    virtual ~Component() = default;

    virtual void setWorld(World* world) final;

private:
    World* m_world;
};

#endif //MOONLANDER_COMPONENT_HPP

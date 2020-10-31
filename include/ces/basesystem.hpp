#ifndef MOONLANDER_BASESYSTEM_HPP
#define MOONLANDER_BASESYSTEM_HPP

class World;

/**
 * System base class
 */
class BaseSystem
{
public:
    BaseSystem(): m_stopped(false), m_world(nullptr) {};
    virtual ~BaseSystem() = default;

    virtual void setWorld(World* world) final
    {
        m_world = world;
    }

    virtual void update(size_t delta) final
    {
        if (!m_stopped)
            update_state(delta);
    }

    virtual void stop() final
    {
        m_stopped = true;
    }

    virtual void start() final
    {
        m_stopped = false;
    }

protected:
    virtual void update_state(size_t delta) = 0;

    World* m_world;
    bool m_stopped;
};

#endif //MOONLANDER_BASESYSTEM_HPP
